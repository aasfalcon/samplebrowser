#define PLUGIN_NAME PluginResamplerLSR
#define PLUGIN_VERSION "1.0.0"

#include "common/plugin.h"
#include "resamplerproviderlsr.h"

PLUGIN_BEGIN
PLUGIN_REGISTER(IResamplerProvider, ResamplerProviderLSR, "1.0.0")
PLUGIN_END
