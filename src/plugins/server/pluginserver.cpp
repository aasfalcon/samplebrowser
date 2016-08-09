#define PLUGIN_NAME PluginServer
#define PLUGIN_DESCRIPTION \
    "Server to manage dynamic linking plugins"

#include "shared/plugin.h"
#include "serverprovider.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IServer, ServerProvider)
PLUGIN_END
