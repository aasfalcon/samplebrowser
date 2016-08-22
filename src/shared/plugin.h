#ifndef PLUGIN_H
#define PLUGIN_H

#include <map>

#include "allocator.h"
#include "iplugin.h"

class Plugin : public IPlugin, public Allocator<Interface> {
    Info _info;

public:
    Plugin(const char* name, const char* version, const char* description);
    ~Plugin();

    const Info* info();
    bool provides(const char* interfaceName);

private:
    std::shared_ptr<std::list<std::string> > _tags;
};

#endif // PLUGIN_H
