#ifndef SERVERPROVIDER_H
#define SERVERPROVIDER_H

#include <list>
#include <map>

#include "iserverprovider.h"

class ServerProvider : public IServerProvider
{
public:
    ServerProvider();
    ~ServerProvider();

    virtual std::list<ProviderInfo> find(
            const std::string &name, unsigned version,
            VersionCompare compare = VersionEqual) const;
    virtual std::list<ProviderInfo> find(
            const std::string &interface) const;
    virtual void initialize(const std::string &path);
    virtual IProvider *newProvider(const ProviderInfo &providerInfo);
    virtual void deleteProvider(IProvider *provider);

private:
    std::list<IServerProvider::PluginInfo> _plugins;
    std::map<const IProvider *, IPlugin *> _providerPlugin;

    static int initPlugin(const char *path, void *data);
};

#endif // SERVERPROVIDER_H
