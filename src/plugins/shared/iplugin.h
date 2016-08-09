#ifndef IPLUGIN_H
#define IPLUGIN_H

#include "interface.h"

struct IPlugin: Interface {
    struct Info {
        const char *description;
        unsigned interfaceCount;
        const char **interfaces;
        const char *name;
        const char *version;
    };

    virtual Interface *create(const char *interfaceName) = 0;
    virtual void destroy(Interface *object) = 0;
    virtual const Info *info() = 0;
    virtual bool provides(const char *interfaceName) = 0;
};

#endif // IPLUGIN_H
