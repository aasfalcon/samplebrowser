#define PLUGIN_NAME PluginAudioFileMPG123
#define PLUGIN_DESCRIPTION \
    "Audio file I/O plugin with MP3 support"

#include "shared/pluginmacros.h"
#include "audiofilempg123.h"

PLUGIN_BEGIN
PLUGIN_PROVIDES(IAudioFile, AudioFileMPG123)
PLUGIN_END
