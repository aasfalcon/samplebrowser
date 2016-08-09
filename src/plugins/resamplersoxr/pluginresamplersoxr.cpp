#define PLUGIN_NAME PluginResamplerSoXR
#define PLUGIN_DESCRIPTION \
    "Resampler plugin builld against soxr-lsr"

#include "shared/plugin.h"
#include "resamplerlsr/resamplerproviderlsr.h"

class ResamplerProviderSoXR: public ResamplerProviderLSR {};

PLUGIN_BEGIN
PLUGIN_PROVIDES(IResampler, ResamplerProviderSoXR)
PLUGIN_PROVIDES(IConverter, ResamplerProviderSoXR)
PLUGIN_END
