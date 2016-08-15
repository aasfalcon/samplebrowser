#ifndef IRESAMPLER_H
#define IRESAMPLER_H

#include "interface.h"
#include "../sound/sound.h"

struct IResampler: Interface
{
    virtual void init(Sound::Quality quality, unsigned channels, unsigned frames, double rate) = 0;
    virtual float *output() = 0;
    virtual unsigned process(const float *input, unsigned inputFramesMax) = 0;
    virtual void reset() = 0;
    virtual void setInputRate(double rate) = 0;
    virtual unsigned simple(float *dest, unsigned dframes,
                            const float *source, unsigned sframes,
                            unsigned channels, double ratio,
                            Sound::Quality quality) = 0;

protected:
    virtual unsigned perform() = 0;
};

#endif // IRESAMPLER_H
