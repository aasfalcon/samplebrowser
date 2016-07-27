#include <sstream>
#include <vector>

#include "plugin.h"

Plugin::Plugin(const std::string &name, unsigned version)
    : _name(name), _version(version)
{
}

Plugin::~Plugin()
{
    for (std::list<IProvider *>::iterator it = _activeProviders.begin();
         it != _activeProviders.end(); ++it)
    {
        delete *it;
    }
}

void Plugin::destroy()
{
    delete this;
}

void Plugin::deleteProvider(IProvider *provider)
{
    for (std::list<IProvider *>::iterator it = _activeProviders.begin();
         it != _activeProviders.end(); ++it)
    {
        if (*it == provider) {
            delete provider;
            _activeProviders.remove(provider);
            break;
        }
    }
}

const std::string &Plugin::name() const
{
    return _name;
}

IProvider *Plugin::newProvider(const std::string &name, unsigned version)
{
    std::list<IServerProvider::ProviderInfo>::iterator it;

    for (it = _providers.begin(); it != _providers.end(); ++it)
    {
        if (it->version == version && it->name == name) {
            IProvider *provider = _constructors[it->name]();
            _activeProviders.push_back(provider);
            return provider;
        }
    }

    return NULL;
}

const std::list<IServerProvider::ProviderInfo> &Plugin::providers() const
{
    return _providers;
}

unsigned Plugin::version() const
{
    return _version;
}

void Plugin::registerProvider(const std::string &interface,
                              const std::string &provider, unsigned version)
{
    IServerProvider::ProviderInfo providerInfo;

    providerInfo.interface = interface;
    providerInfo.name = provider;
    providerInfo.version = version;
    providerInfo.plugin = this;
    _providers.push_back(providerInfo);
}
