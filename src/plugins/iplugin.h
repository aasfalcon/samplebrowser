#ifndef IPLUGIN
#define IPLUGIN

#include <list>
#include <string>

#include "iserverprovider.h"

class IPlugin : public Interface {
public:
    virtual void destroy() = 0;
    virtual void deleteProvider(IProvider *provider) = 0;
    virtual const std::string &name() const = 0;
    virtual IProvider *newProvider(const std::string &name, unsigned version) = 0;
    virtual const std::list<IServerProvider::ProviderInfo> &providers() const = 0;
    virtual unsigned version() const = 0;
};

#endif // IPLUGIN
