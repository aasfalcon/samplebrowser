#define PLUGIN_NAME PluginResamplerLSR
#define PLUGIN_DESCRIPTION \
    "Resampler plugin builld ageinst libsamplerate library"

#include "shared/plugin.h"
#include "resamplerproviderlsr.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IResampler, ResamplerProviderLSR)
PLUGIN_PROVIDES(IConverter, ResamplerProviderLSR)
PLUGIN_END
