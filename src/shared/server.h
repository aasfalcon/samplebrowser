#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "version.h"

struct Interface;
struct IPlugin;
struct lt__handle;

class Server {
public:
    virtual ~Server();
    static std::shared_ptr<Server> instance();

    template<typename T>
    std::shared_ptr<T> factory(const std::string &interfaceName);
    std::string pluginsPath();
    void prefer(const std::string &interfaceName, const std::string &pluginName,
                Version pluginVersion = Version(),
                Version::Compare compare = Version::CompareGreaterOrEqual);
    void shutdown();
    void startup(const std::string &pluginsPath);

private:
    struct Preference {
        Version::Compare compare;
        std::string pluginName;
        Version version;
    };

    static std::shared_ptr<Server> _instance;
    std::map<std::shared_ptr<lt__handle>, std::shared_ptr<IPlugin>> _modules;
    std::string _pluginsPath;
    std::map<std::string, Preference> _preferences;

    Server();
    std::shared_ptr<Interface> create(const std::string &interfaceName,
                                      const Preference *preference = 0);
    IPlugin *findPlugin(const std::string &interfaceName,
                        const Preference *preference);
};

template<typename T>
std::shared_ptr<T> Server::factory(const std::string &interfaceName) {
    return std::static_pointer_cast<T>(create(interfaceName));
}

#define PLUGIN_FACTORY(__interface) \
    Server::instance()->factory<__interface>(#__interface)

#endif // SERVER_H
