#ifndef SOUND_PROCESSOR_REGISTER_H
#define SOUND_PROCESSOR_REGISTER_H

#ifndef SOUND_REGISTER_PROCESSOR
#error "This file is intended for Base class initialization"
#endif

// processor class heaaders
#include "player.h"
#include "silence.h"
#include "resampler.h"

// registration macro
#define SOUND_REGISTER_PROCESSORS      \
    SOUND_REGISTER_PROCESSOR(Player);  \
    SOUND_REGISTER_PROCESSOR(Silence); \
    SOUND_REGISTER_PROCESSOR(Resampler);

#endif // SOUND_PROCESSOR_REGISTER_H
