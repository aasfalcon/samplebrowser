#define PLUGIN_NAME PluginAudioFileSndfile
#define PLUGIN_DESCRIPTION \
    "Audio file I/O plugin (libsndfile wrapper)"

#include "shared/pluginmacros.h"
#include "audiofilesndfile.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IAudioFile, AudioFileSndfile)
PLUGIN_END
