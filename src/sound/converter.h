#ifndef CONVERTER_H
#define CONVERTER_H

#include "sound.h"
#include <memory>

struct IResampler;

class Converter
{
public:
    static void convert(void *dest, Sound::Type dtype,
                        const void *source, Sound::Type stype,
                        unsigned count);

    static void resample(void *dest, unsigned destFrames,
                         const void *source, unsigned sourceFrames,
                         unsigned channels, double ratio,
                         Sound::Type type, Sound::Quality quality);

private:
    typedef void (*Convert)(void *, const void *, unsigned);

    static const Convert matrix[Sound::TypeCount][Sound::TypeCount];
    static const unsigned sizeMap[Sound::TypeCount];
    static std::shared_ptr<IResampler> resampler;
};

#endif // CONVERTER_H
