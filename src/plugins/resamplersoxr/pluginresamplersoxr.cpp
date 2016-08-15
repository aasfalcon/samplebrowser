#define PLUGIN_NAME PluginResamplerSoXR
#define PLUGIN_DESCRIPTION \
    "Resampler plugin builld against soxr-lsr"

#include "shared/pluginmacros.h"
#include "plugins/resamplerlsr/resamplerlsr.h"

class ResamplerSoXR: public ResamplerLSR {};

PLUGIN_BEGIN
PLUGIN_PROVIDES(IResampler, ResamplerSoXR)
PLUGIN_END
