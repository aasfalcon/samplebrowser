#ifndef SERVERPROVIDER_H
#define SERVERPROVIDER_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "shared/iserver.h"

struct lt__handle;
struct IPlugin;

class ServerProvider: public IServer
{
public:
    ServerProvider();
    ~ServerProvider();

    virtual Interface *create(const char *interfaceName,
                              const Preference *preference = 0);
    virtual void destroy(Interface *object);
    virtual void poll(const char *pluginsPath);
    virtual void prefer(const char *interfaceName, const char *pluginName,
                        const char *version = "0.0.0",
                        Version::Compare compare = Version::CompareGreaterOrEqual,
                        bool strict = false);
private:
    struct InternalPreference {
        Version::Compare compare;
        std::string pluginName;
        Version version;
    };

    std::list<std::shared_ptr<lt__handle>> _modules;
    std::map<Interface *, IPlugin *> _objectToPlugin;
    std::list<std::shared_ptr<IPlugin>> _plugins;
    std::string _pluginsPath;
    std::map<std::string, InternalPreference> _preferences;

    IPlugin *findPlugin(const char *interfaceName,
                        const InternalPreference *preference);

};

#endif // SERVERPROVIDER_H
