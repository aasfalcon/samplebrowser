#ifndef CONVERTER_H
#define CONVERTER_H

#include <memory>
#include "sound.h"

template<typename T>
class Buffer;

struct IResampler;

class Converter
{
public:
    virtual ~Converter();
    void convert(void *dest, Sound::Type dtype, const void *source,
                 Sound::Type stype, unsigned count);
    static Converter &instance();
    void real24bit(void *dest, const Buffer<Sound::Int24> &source);
    void resample(void *dest, unsigned destFrames, const void *source,
                  unsigned sourceFrames, unsigned channels, double ratio,
                  Sound::Type type, Sound::Quality quality);

private:
    Converter();

    typedef void (*Convert)(void *, const void *, unsigned);

    static std::unique_ptr<Converter> _instance;
    const Convert _matrix[Sound::TypeCount][Sound::TypeCount];
    std::shared_ptr<IResampler> _resampler;
    const unsigned _sizeMap[Sound::TypeCount];
};

#endif // CONVERTER_H
