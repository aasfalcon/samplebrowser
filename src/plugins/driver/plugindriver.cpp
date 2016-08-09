#define PLUGIN_NAME PluginDriver
#define PLUGIN_DESCRIPTION \
    "RtAudio realtime audio driver support plugin"

#include "shared/plugin.h"
#include "driverprovider.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IDriver, DriverProvider)
PLUGIN_END
