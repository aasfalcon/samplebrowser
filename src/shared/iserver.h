#ifndef ISERVER_H
#define ISERVER_H

#include "interface.h"
#include "shared/version.h"

struct IServer: Interface {
    struct Preference {
        Version::Compare compare;
        const char *pluginName;
        const char *version;
    };

    virtual Interface *create(const char *interfaceName,
                              const Preference *preference = 0) = 0;
    virtual void destroy(Interface *object) = 0;
    virtual void poll(const char *pluginsPath) = 0;
    virtual void prefer(const char *interfaceName, const char *pluginName,
                        const char *version = "0.0.0",
                        Version::Compare compare = Version::CompareGreaterOrEqual,
                        bool strict = false) = 0;
};

#endif // ISERVER_H
