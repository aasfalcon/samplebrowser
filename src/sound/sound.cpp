#include "sound.h"

template<typename T>
Object<T>::~Object()
{

}

#define SAMPLE_TYPE(__type) \
    template<> Sound::Type Object<Sound::__type>::type() \
    { \
        return Sound::Type##__type; \
    }

SOUND_SPECIALIZE(SAMPLE_TYPE)
