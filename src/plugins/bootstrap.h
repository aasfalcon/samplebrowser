#ifndef BOOTSTRAP
#define BOOTSTRAP

#include <string>

#include <ltdl.h>

#include "iplugin.h"
#include "iserverprovider.h"
#include "version.h"

#ifndef PLUGIN_ROOT
#define PLUGIN_ROOT "./"
#endif

#ifndef PLUGIN_SERVER_PROVIDER_VERSION
#define PLUGIN_SERVER_PROVIDER_VERSION "1.0.0"
#endif

#ifndef PLUGIN_SERVER_PROVIDER
#define PLUGIN_SERVER_PROVIDER "Server"
#endif

#ifndef PLUGIN_SERVER_MODULE
#define PLUGIN_SERVER_MODULE "server"
#endif

class Bootstrap {
public:
    static IServerProvider *initialize() {
        if (!_Instance) {
            _Instance = new Bootstrap(
                        PLUGIN_ROOT,
                        PLUGIN_SERVER_MODULE,
                        PLUGIN_SERVER_PROVIDER,
                        Version::encode(PLUGIN_SERVER_PROVIDER_VERSION));
        }

        return _Instance->_server;
    }

    static void shutdown() {
        if (_Instance) {
            delete _Instance;
            _Instance = NULL;
        }
    }

private:
    Bootstrap(const std::string &path,
                     const std::string &module,
                     const std::string &provider,
                     unsigned version)
        : _plugin(NULL)
        , _server(NULL)
    {
        lt_dlinit();
        lt_dlsetsearchpath(path.c_str());
        lt_dlhandle handle = lt_dlopen(module.c_str());

        if (!handle) {
            return;
        }

        IPlugin *(*create)() = (IPlugin *(*)())
                (lt_dlsym(handle, "plugin_create"));

        if (!create) {
            lt_dlclose(handle);
            lt_dlexit();
            return;
        }

        _plugin = (*create)();

        if (!_plugin) {
            lt_dlclose(handle);
            lt_dlexit();
            return;
        }

        _server = static_cast<IServerProvider *>(
                    _plugin->newProvider(provider, version));

        if (!_server) {
            _plugin->destroy();
            _plugin = NULL;
            lt_dlclose(handle);
            lt_dlexit();
        }
    }

    ~Bootstrap()
    {
        if (_plugin) {
            _plugin->destroy();
        }

        lt_dlexit();
    }

private:
    static Bootstrap *_Instance;
    IPlugin *_plugin;
    IServerProvider *_server;
};

Bootstrap *Bootstrap::_Instance;

#endif // BOOTSTRAP
