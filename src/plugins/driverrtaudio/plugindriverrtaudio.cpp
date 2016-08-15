#define PLUGIN_NAME PluginDriverRtAudio
#define PLUGIN_DESCRIPTION \
    "RtAudio realtime audio driver support plugin"

#include "shared/pluginmacros.h"
#include "driverrtaudio.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IDriver, DriverRtAudio)
PLUGIN_END
