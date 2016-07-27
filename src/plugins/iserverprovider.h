#ifndef ISERVERPROVIDER_H
#define ISERVERPROVIDER_H

#include <list>

#include "iprovider.h"

class IPlugin;

class IServerProvider : public IProvider {
public:
    struct ProviderInfo {
        std::string interface;
        std::string name;
        IPlugin *plugin;
        unsigned version;
    };

    struct PluginInfo {
        std::string path;
        IPlugin *plugin;
    };

    enum VersionCompare {
        VersionEqual,
        VersionLowerThan,
        VersionLowerThanOrEqual,
        VersionGreaterThan,
        VersionGreaterThanOrEqual,
    };

    virtual std::list<ProviderInfo> find(
            const std::string &name, unsigned version,
            VersionCompare compare = VersionEqual) const = 0;
    virtual std::list<ProviderInfo> find(
            const std::string &interface) const = 0;
    virtual void initialize(const std::string &path) = 0;
    virtual IProvider *newProvider(const ProviderInfo &providerInfo) = 0;
    virtual void deleteProvider(IProvider *provider) = 0;
};

#endif // ISERVERPROVIDER_H
