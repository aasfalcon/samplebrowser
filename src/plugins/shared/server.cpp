#include <stdexcept>
#include <sstream>
#include <ltdl.h>

#include "shared/iplugin.h"
#include "shared/iserver.h"
#include "server.h"
#include "version.h"

std::shared_ptr<Server> Server::_instance;

Server::Server()
{
    lt_dlinit();
}

Server::~Server()
{
    lt_dlexit();
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
    _provider->prefer(interfaceName.c_str(), pluginName.c_str(),
                      pluginVersion.str().c_str(), compare);
}

void Server::shutdown() {
    if (_instance) {
        _instance.reset();
    }
}

void Server::startup(const std::string &pluginsPath,
                     const std::string &moduleName,
                     const std::string &moduleInterface,
                     const std::string &modulePath)
{
    lt_dlsetsearchpath((modulePath.empty() ? modulePath : pluginsPath).c_str());
    _plugin.reset();
    _handle = std::shared_ptr<lt__handle>(
                lt_dlopenext(moduleName.c_str()),
                lt_dlclose);

    if (!_handle) {
        std::ostringstream message;
        message << "Can't load server module [" << moduleName << ']'
                << " from path: " << modulePath << '\n'
                << "LTDL:" << lt_dlerror();
        throw std::runtime_error(message.str());
    }

    auto plugin_create = (IPlugin *(*)())
            lt_dlsym(_handle.get(), "plugin_create");
    auto plugin_destroy = (void (*)(IPlugin *))
            lt_dlsym(_handle.get(), "plugin_destroy");

    if (!plugin_create || !plugin_destroy) {
        std::ostringstream message;
        message << "Can't import [plugin_create] and [plugin_destroy] symbols"
                << " from module [" << moduleName << "]\n"
                << "LTDL:" << lt_dlerror();
        throw std::runtime_error(message.str());
    }

    _plugin = std::shared_ptr<IPlugin>(
                (*plugin_create)(),
                plugin_destroy);

    if (!_plugin) {
        throw std::runtime_error("Error creating server plugin object");
    }

    Interface *pinterface = _plugin->create(moduleInterface.c_str());
    _provider = std::shared_ptr<IServer>(
                dynamic_cast<IServer *>(pinterface),
                [this](IServer *interface) {
                    this->_plugin->destroy(interface);
                }
            );
    _provider->poll(pluginsPath.c_str());
}
