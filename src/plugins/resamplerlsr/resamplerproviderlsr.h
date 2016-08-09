#ifndef RESAMPLERPROVIDERLSR_H
#define RESAMPLERPROVIDERLSR_H

#include "shared/iresampler.h"
#include "../sound/sound.h"

struct SRC_STATE_tag;

class ResamplerProviderLSR: public IResampler
{
public:
    ResamplerProviderLSR();
    virtual ~ResamplerProviderLSR();
    virtual void init(Sound::Quality quality, unsigned channels,
                      unsigned frames, double rate);
    float *output();
    unsigned process(const float *input, unsigned inputFramesMax);
    void reset();
    void setInputRate(double rate);
    virtual unsigned simple(float *dest, unsigned dframes,
                            const float *source, unsigned sframes,
                            unsigned channels, double ratio,
                            Sound::Quality quality);

private:
    unsigned _channels;
    unsigned _frames;
    const float *_input;
    unsigned _inputFramesMax;
    double _inputRate;
    float *_output;
    double _outputRate;
    SRC_STATE_tag *_state;
    static int _types[Sound::QualityCount];

    unsigned perform();
};

#endif // RESAMPLERPROVIDERLSR_H
