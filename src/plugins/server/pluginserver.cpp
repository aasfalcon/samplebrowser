#define PLUGIN_NAME PluginServer

#include "common/plugin.h"
#include "serverprovider.h"

PLUGIN_BEGIN
PLUGIN_REGISTER(IServerProvider, ServerProvider, 1.0.0)
PLUGIN_END
