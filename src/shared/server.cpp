#include <stdexcept>
#include <sstream>
#include <ltdl.h>

#include "server.h"
#include "version.h"
#include "shared/iplugin.h"
#include "shared/log.h"

std::shared_ptr<Server> Server::_instance;

Server::Server()
{
    lt_dlinit();
}

Server::~Server()
{
    lt_dlexit();
}

std::shared_ptr<Interface> Server::create(const std::string &interfaceName,
                                          const Preference *preference)
{
    if (!preference) {
        auto it = _preferences.find(interfaceName);

        if (it != _preferences.end()) {
            preference = &it->second;
        }
    }

    IPlugin *plugin = this->findPlugin(interfaceName, preference);
    return std::shared_ptr<Interface>(
                plugin->create(interfaceName.c_str()),
                [plugin](Interface *object) {
                    plugin->destroy(object);
                });
}

IPlugin *Server::findPlugin(const std::string &interfaceName,
                            const Preference *preference)
{
    for (auto it = _modules.cbegin(); it != _modules.cend(); ++it) {
        IPlugin *plugin = it->second.get();

        if (plugin->provides(interfaceName.c_str())) {
            if (!preference) {
                return plugin;
            }

            const IPlugin::Info *info = plugin->info();

            if (preference->pluginName == info->name) {
                Version ver(info->version);

                if (ver.pass(preference->compare, preference->version)) {
                    return plugin;
                }
            }
        }
    }

    std::ostringstream ss;
    ss << "Can't find plugin";

    if (preference) {
        ss << " [" << preference->pluginName
                << Version::sign(preference->compare)
                << preference->version.str() << ']';
    }

    ss << " providing interface "
            << '[' << interfaceName << ']'
            << " in path: " << _pluginsPath;
    RUNTIME_ERROR(ss.str());
}

std::shared_ptr<Server> Server::instance()
{
    if (!_instance) {
        _instance = std::shared_ptr<Server>(new Server);
    }

    return _instance;
}

std::string Server::pluginsPath()
{
    return _pluginsPath;
}

void Server::prefer(const std::string &interfaceName, const std::string &pluginName,
                    Version pluginVersion, Version::Compare compare)
{
    Preference preference = { compare, pluginName, pluginVersion };

    try {
        findPlugin(interfaceName, &preference);
    } catch(...) {
        return;
    }

    _preferences[interfaceName] = preference;
}

void Server::shutdown() {
    if (_instance) {
        _instance.reset();
    }
}

void Server::startup(const std::string &pluginsPath)
{
    auto pollPlugin = [](const char *path, void *pthis) -> int
    {
        std::shared_ptr<lt__handle> handle(
                    lt_dlopenext(path),
                    lt_dlclose);

        if (handle) {
            auto plugin_create = reinterpret_cast<IPlugin *(*)()>(
                    lt_dlsym(handle.get(), "plugin_create"));
            auto plugin_destroy = reinterpret_cast<void (*)(IPlugin *)>(
                    lt_dlsym(handle.get(), "plugin_destroy"));

            if (plugin_create && plugin_destroy) {
                IPlugin *plugin = (*plugin_create)();
                Server *server = static_cast<Server *>(pthis);
                server->_modules[handle] = std::shared_ptr<IPlugin>(plugin, plugin_destroy);
            }
        }

        return 0;
    };

    lt_dlsetsearchpath(pluginsPath.c_str());
    lt_dlforeachfile(pluginsPath.c_str(), pollPlugin, this);
    _pluginsPath = pluginsPath;
}
