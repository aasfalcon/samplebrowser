#define PLUGIN_NAME PluginAudioFile
#define PLUGIN_DESCRIPTION \
    "Audio file I/O plugin"

#include "shared/plugin.h"
#include "audiofileprovider.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IAudioFile, AudioFileProvider)
PLUGIN_END
