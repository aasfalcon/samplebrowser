#define PLUGIN_NAME PluginAudioFile
#define PLUGIN_DESCRIPTION \
    "Audio file I/O plugin"

#include "shared/plugin.h"
#include "audiofileprovidermpg123.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IAudioFile, AudioFileProviderMpg123)
PLUGIN_END
