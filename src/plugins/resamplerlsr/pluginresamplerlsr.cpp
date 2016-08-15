#define PLUGIN_NAME PluginResamplerLSR
#define PLUGIN_DESCRIPTION \
    "Resampler plugin builld ageinst libsamplerate library"

#include "shared/pluginmacros.h"
#include "resamplerproviderlsr.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IResampler, ResamplerProviderLSR)
PLUGIN_END
