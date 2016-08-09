#include <cstring>
#include <sstream>
#include <ltdl.h>

#include "shared/iplugin.h"
#include "serverprovider.h"

#define PLUGIN_NAME
#define PLUGIN_DESCRIPTION
#include "shared/plugin.h"

ServerProvider::ServerProvider()
{
    lt_dlinit();
}

ServerProvider::~ServerProvider()
{
    lt_dlexit();
}

Interface *ServerProvider::create(const char *interfaceName,
                                  const IServer::Preference *preference)
{
    InternalPreference ip;
    InternalPreference *ipp = NULL;

    if (preference) {
        ip = {
            preference->compare,
            preference->pluginName,
            preference->version,
        };

        ipp = &ip;
    } else {
        auto it = _preferences.find(interfaceName);

        if (it != _preferences.end()) {
            ipp = &it->second;
        }
    }

    IPlugin *plugin = this->findPlugin(interfaceName, ipp);
    Interface *object = plugin->create(interfaceName);
    _objectToPlugin[object] = plugin;
    return object;
}

void ServerProvider::destroy(Interface *object)
{
    IPlugin *plugin = _objectToPlugin.at(object);
    _objectToPlugin.erase(_objectToPlugin.find(object));
    plugin->destroy(object);
}

IPlugin *ServerProvider::findPlugin(const char *interfaceName,
                                    const InternalPreference *preference)
{
    for (auto it = _plugins.begin(); it != _plugins.end(); ++it) {
        IPlugin *plugin = it->get();

        if (plugin->provides(interfaceName)) {
            if (!preference) {
                return plugin;
            }

            const IPlugin::Info *info = (*it)->info();

            if (preference->pluginName == info->name) {
                Version ver(info->version);

                if (ver.pass(preference->compare, preference->version)) {
                    return plugin;
                }
            }
        }
    }

    std::ostringstream message;
    message << "Can't find plugin";

    if (preference) {
        message << " [" << preference->pluginName
                << Version::sign(preference->compare)
                << preference->version.str() << ']';
    }

    message << " providing interface "
            << '[' << interfaceName << ']'
            << " in path: " << _pluginsPath;
    throw std::runtime_error(message.str());
}

void ServerProvider::poll(const char *pluginsPath)
{
    _plugins.clear();

    auto pollPlugin = [](const char *path, void *pthis) -> int
    {
        std::shared_ptr<lt__handle> handle(
                    lt_dlopenext(path),
                    lt_dlclose);

        if (handle) {
            auto plugin_create = (IPlugin *(*)())
                    lt_dlsym(handle.get(), STRINGIFY(PLUGIN_CREATE));
            auto plugin_destroy = (void (*)(IPlugin *))
                    lt_dlsym(handle.get(), STRINGIFY(PLUGIN_DESTROY));

            if (plugin_create && plugin_destroy) {
                IPlugin *plugin = (*plugin_create)();
                ServerProvider *server = (ServerProvider *)pthis;
                server->_plugins.push_back(
                            std::shared_ptr<IPlugin>(plugin, plugin_destroy));
                server->_modules.push_back(handle);
            }
        }

        return 0;
    };

    lt_dlsetsearchpath(pluginsPath);
    lt_dlforeachfile(pluginsPath, pollPlugin, this);
    _pluginsPath = pluginsPath;
}

void ServerProvider::prefer(const char *interfaceName, const char *pluginName,
                            const char *version, Version::Compare compare, bool strict)
{
    InternalPreference preference = { compare, pluginName, Version(version) };

    try {
        findPlugin(interfaceName, &preference);
    } catch(...) {
        if (strict) {
            throw;
        }

        return;
    }

    _preferences[interfaceName] = preference;
}
