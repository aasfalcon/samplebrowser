#ifndef PLUGIN_H
#define PLUGIN_H

#include <map>

#include "allocator.h"
#include "iplugin.h"

class Plugin : public IPlugin, protected Allocator<Interface> {
    Info _info;

public:
    Plugin(const char* name, const char* version, const char* description);
    ~Plugin();

    Interface *create(const char *interfaceName) override;
    void destroy(Interface *object) override;
    const Info* info() override;
    bool provides(const char* interfaceName) override;

private:
    std::shared_ptr<std::list<std::string> > _tags;
};

#endif // PLUGIN_H
