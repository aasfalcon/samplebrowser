#ifndef PLUGIN_H
#define PLUGIN_H

#include <map>

#include "allocator.h"

class Plugin: public Allocator
{
    Info _info;

public:
    Plugin(const char *name, const char *version, const char *description);
    ~Plugin();

    const Info *info();
    bool provides(const char *interfaceName);
};

#endif // PLUGIN_H
