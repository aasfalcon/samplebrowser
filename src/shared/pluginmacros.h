#ifndef PLUGINMACROS_H
#define PLUGINMACROS_H

#include "log.h"
#include "plugin.h"

#define STRINGIFY_(__x) #__x
#define STRINGIFY(__x) STRINGIFY_(__x)
#define PLUGIN_CREATE plugin_create
#define PLUGIN_DESTROY plugin_destroy

#define PLUGIN_BEGIN \
    class PLUGIN_NAME: public Plugin { \
    public: \
        ~PLUGIN_NAME() \
        { \
            LOG(TRACE, "Destroying plugin object"); \
        } \
        PLUGIN_NAME() \
            : Plugin(STRINGIFY(PLUGIN_NAME), PLUGIN_VERSION, PLUGIN_DESCRIPTION) \
        { \
            LOG(TRACE, "Creating plugin object"); \
            Create create; \
            Destroy destroy;

#define PLUGIN_PROVIDES(__interface, __provider) \
            create = []() -> Interface * { \
                LOG(TRACE, "Allocating " << #__provider << " [" << #__interface << ']'); \
                return new __provider; \
            }; \
            destroy = [](Interface *ptr) { \
                LOG(TRACE, "Freeing " << #__provider << " [" << #__interface << ']'); \
                auto object = dynamic_cast<__provider *>(ptr); \
                delete object; \
            }; \
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

#endif // PLUGINMACROS_H
