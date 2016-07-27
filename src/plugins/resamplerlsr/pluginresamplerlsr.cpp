#define PLUGIN_NAME PluginResamplerLSR

#include "common/plugin.h"
#include "resamplerproviderlsr.h"

PLUGIN_BEGIN
PLUGIN_REGISTER(IResamplerProvider, ResamplerProviderLSR, 1.0.0)
PLUGIN_END
