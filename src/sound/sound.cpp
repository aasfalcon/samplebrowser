#include "sound.h"

template<typename T>
Object<T>::~Object()
{

}

#define __OBJECT_TYPE(__type) \
    template<> Sound::Type Object<Sound::__type>::type() \
    { \
        return Sound::Type##__type; \
    }

SOUND_INSTANTIATE(Object)
SOUND_SPECIALIZE(__OBJECT_TYPE)
