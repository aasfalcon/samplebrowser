#ifndef RESAMPLERPROVIDERLSR_H
#define RESAMPLERPROVIDERLSR_H

#include "shared/iresampler.h"
#include "sound/object.h"

struct SRC_STATE_tag;

class ResamplerLSR: public IResampler
{
public:
    ResamplerLSR();
    ~ResamplerLSR();

    void init(Quality quality, unsigned channels,
                      unsigned frames, double rate);
    float *output();
    unsigned process(const float *input, unsigned inputFramesMax);
    void reset();
    void setInputRate(double rate);
    unsigned simple(float *dest, unsigned dframes,
                    const float *source, unsigned sframes,
                    unsigned channels, double ratio,
                    Quality quality);
protected:
    unsigned perform();

private:
    unsigned _channels;
    unsigned _frames;
    const float *_input;
    unsigned _inputFramesMax;
    double _inputRate;
    float *_output;
    double _outputRate;
    SRC_STATE_tag *_state;
    static int _types[QualityCount];
};

#endif // RESAMPLERPROVIDERLSR_H
