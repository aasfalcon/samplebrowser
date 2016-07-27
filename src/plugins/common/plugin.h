#ifndef PLUGIN_H
#define PLUGIN_H

#include <functional>
#include <list>
#include <map>

#include "iplugin.h"
#include "version.h"

class Plugin : public IPlugin
{
public:
    Plugin(const std::string &name, unsigned version);
    ~Plugin();

    virtual void destroy();
    void deleteProvider(IProvider *provider);
    const std::string &name() const;
    IProvider *newProvider(const std::string &name, unsigned version);
    const std::list<IServerProvider::ProviderInfo> &providers() const;
    unsigned version() const;

protected:
    std::list<IProvider *>_activeProviders;
    std::map<std::string, std::function<IProvider *()>> _constructors;
    std::list<IServerProvider::ProviderInfo> _providers;
    std::string _name;
    unsigned _version;

    void registerProvider(const std::string &interface,
                          const std::string &provider, unsigned version);
};

#define PLUGIN_BEGIN_(plugin, version) \
    class plugin : public Plugin { \
    public: \
        inline plugin() \
            : Plugin(#plugin, Version::encode(#version)) \
        { \
            IServerProvider::ProviderInfo pri;

#define PLUGIN_REGISTER(interface, provider, version) \
            registerProvider(#interface, #provider, \
                             Version::encode(#version)); \
            _constructors[#provider] = []() { return new provider; };

#define PLUGIN_END_(plugin) \
        } \
    }; \
    extern IPlugin *plugin_create() { \
        return new plugin; };

#define PLUGIN_BEGIN PLUGIN_BEGIN_(PLUGIN_NAME, PLUGIN_VERSION)
#define PLUGIN_END PLUGIN_END_(PLUGIN_NAME)

#ifndef PLUGIN_VERSION
#define PLUGIN_VERSION 1.0.0
#endif

#endif // PLUGIN_H
