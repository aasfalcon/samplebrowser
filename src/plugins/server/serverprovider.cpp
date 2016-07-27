#include <string>

#include <ltdl.h>

#include "common/plugin.h"
#include "serverprovider.h"

ServerProvider::ServerProvider()
{
    lt_dlinit();
}

ServerProvider::~ServerProvider()
{
    std::map<const IProvider *, IPlugin *>::iterator it;

    for (it = _providerPlugin.begin(); it != _providerPlugin.end(); ++it) {
        IPlugin *plugin = it->second;
        plugin->destroy();
    }

    lt_dlexit();
}

std::list<IServerProvider::ProviderInfo> ServerProvider::find(
        const std::string &name,
        unsigned version,
        IServerProvider::VersionCompare compare) const
{
    std::list<IServerProvider::ProviderInfo> result;
    std::list<IServerProvider::PluginInfo>::const_iterator plit;

    for (plit = _plugins.begin(); plit != _plugins.end(); ++plit) {
        std::list<IServerProvider::ProviderInfo>::const_iterator prit;

        for (prit = plit->plugin->providers().begin();
             prit != plit->plugin->providers().end(); ++prit)
        {
            if (prit->name == name && (
                (compare == VersionEqual && prit->version == version)
                || (compare == VersionGreaterThan && prit->version > version)
                || (compare == VersionGreaterThanOrEqual && prit->version >= version)
                || (compare == VersionLowerThan && prit->version < version)
                || (compare == VersionLowerThanOrEqual && prit->version <= version)
                )
            ) {
                result.push_back(*prit);
            }
        }
    }

    return result;
}

std::list<IServerProvider::ProviderInfo> ServerProvider::find(const std::string &interface) const
{
    std::list<IServerProvider::ProviderInfo> result;
    std::list<IServerProvider::PluginInfo>::const_iterator it;

    for (it = _plugins.begin(); it != _plugins.end(); ++it) {
        std::list<IServerProvider::ProviderInfo>::const_iterator pit;

        for (pit = it->plugin->providers().begin();
             pit != it->plugin->providers().end(); ++pit)
        {
            if (pit->interface == interface) {
                result.push_back(*pit);
            }
        }
    }

    return result;
}

int ServerProvider::initPlugin(const char *path, void *data) {
    ServerProvider *server = static_cast<ServerProvider *>(data);
    lt_dlhandle handle = lt_dlopen(path);

    if (handle) {
        IPlugin *(*create)() = (IPlugin *(*)())
                lt_dlsym(handle, "plugin_create");

        if (create) {
            IPlugin *plugin = (*create)();

            if (plugin) {
                PluginInfo pli;
                pli.path = path;
                pli.plugin = plugin;
                server->_plugins.push_back(pli);
            }
        }
    }

    return 0;
}

void ServerProvider::initialize(const std::string &path)
{
    lt_dlforeachfile(path.c_str(), &ServerProvider::initPlugin, this);
}

IProvider *ServerProvider::newProvider(const ProviderInfo &providerInfo)
{
    IPlugin *plugin = (IPlugin *)providerInfo.plugin;
    IProvider *provider = plugin->newProvider(providerInfo.name,
                                              providerInfo.version);
    _providerPlugin[provider] = plugin;
    return provider;
}

void ServerProvider::deleteProvider(IProvider *provider)
{
    IPlugin *plugin = _providerPlugin[provider];
    _providerPlugin.erase(_providerPlugin.find(provider));
    plugin->deleteProvider(provider);
}
