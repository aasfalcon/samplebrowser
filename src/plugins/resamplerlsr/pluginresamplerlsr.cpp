#define PLUGIN_NAME PluginResamplerLSR
#define PLUGIN_DESCRIPTION \
    "Resampler plugin builld ageinst libsamplerate library"

#include "resamplerlsr.h"
#include "shared/pluginmacros.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IResampler, ResamplerLSR)
PLUGIN_END
