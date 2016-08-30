#ifndef SOUND_DITHER_H
#define SOUND_DITHER_H

#include <cmath>
#include <random>

#include "object.h"

namespace Sound {

class Dither {
public:
    inline static Precise dither(Precise value)
    {
        return value + tpdf();
    }

    inline static Precise round(Precise value)
    {
        return std::roundl(value);
    }

    inline static Precise tpdf()
    {
        return _pdis(_generator) + _ndis(_generator);
    }

private:
    static std::random_device _device;
    static std::mt19937_64 _generator;
    static std::uniform_real_distribution<Precise> _ndis;
    static std::uniform_real_distribution<Precise> _pdis;
};
}

#endif // SOUND_DITHER_H
