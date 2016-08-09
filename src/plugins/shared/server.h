#ifndef SERVER_H
#define SERVER_H

#define PLUGIN_SERVER_MODULE "libserver"
#define PLUGIN_SERVER_INTERFACE "IServer"

#include <memory>
#include "shared/iserver.h"

struct Interface;
struct IPlugin;
struct lt__handle;

class Server {
public:
    ~Server();
    static std::shared_ptr<Server> instance();

    template<typename T>
    inline std::shared_ptr<T> factory(const std::string &interfaceName) {
        Interface *provider = _provider->create(interfaceName.c_str());
        return std::shared_ptr<T>(
                    dynamic_cast<T *>(provider),
                    [this](Interface *object) {
                        this->_provider->destroy(object);
                    }
                );
    }

    std::string pluginsPath();
    void prefer(const std::string &interfaceName, const std::string &pluginName,
                Version pluginVersion = Version(),
                Version::Compare compare = Version::CompareGreaterOrEqual);
    void shutdown();
    void startup(const std::string &pluginsPath,
                 const std::string &moduleName = PLUGIN_SERVER_MODULE,
                 const std::string &moduleInterface = PLUGIN_SERVER_INTERFACE,
                 const std::string &modulePath = "");

private:
    static std::shared_ptr<Server> _instance;
    std::shared_ptr<lt__handle> _handle;
    std::shared_ptr<IPlugin> _plugin;
    std::string _pluginsPath;
    std::shared_ptr<IServer> _provider;

    Server();

};

#define PLUGIN_FACTORY(__interface) \
    Server::instance()->factory<__interface>(#__interface)

#endif // SERVER_H
