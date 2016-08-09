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

    virtual const Info *info();
    virtual bool provides(const char *interfaceName);
};

#define STRINGIFY_(__x) #__x
#define STRINGIFY(__x) STRINGIFY_(__x)
#define PLUGIN_CREATE plugin_create
#define PLUGIN_DESTROY plugin_destroy

#ifdef NO_DEBUG
#define PLUGIN_DEBUG(__message)
#else
#include <iostream>
#define PLUGIN_DEBUG(__message) \
    std::cout << '[' << STRINGIFY(PLUGIN_NAME) << "]: " << __message << '\n'; \
    std::cout.flush();
#endif


#define PLUGIN_BEGIN \
    class PLUGIN_NAME: public Plugin { \
    public: \
        ~PLUGIN_NAME() \
        { \
            PLUGIN_DEBUG("finalization"); \
        } \
        PLUGIN_NAME() \
            : Plugin(STRINGIFY(PLUGIN_NAME), PLUGIN_VERSION, PLUGIN_DESCRIPTION) \
        { \
            PLUGIN_DEBUG("initialization"); \
            Create create; \
            Destroy destroy;

#define PLUGIN_PROVIDES(__interface, __provider) \
            create = []() -> Interface * { \
                PLUGIN_DEBUG("providing interface [" << #__interface << "] with [" << #__provider << ']'); \
                return new __provider; \
            }; \
            destroy = [](Interface *object) { delete object; }; \
            addTag(#__interface, create, destroy);

#define PLUGIN_END \
        } \
    }; \
    extern "C" IPlugin *PLUGIN_CREATE() { return new PLUGIN_NAME; }; \
    extern "C" void PLUGIN_DESTROY(IPlugin *plugin) { delete plugin; };

#ifndef PLUGIN_NAME
#error Define PLUGIN_NAME before including this file
#endif

#ifndef PLUGIN_DESCRIPTION
#error Define PLUGIN_DESCRIPTION string before including this file
#endif

#ifndef PLUGIN_VERSION
#error PLUGIN_VERSION not defined, check your compiler flags
#endif

#endif // PLUGIN_H
