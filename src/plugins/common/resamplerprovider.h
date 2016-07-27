#ifndef RESAMPLERPROVIDER_H
#define RESAMPLERPROVIDER_H

#include "iresamplerprovider.h"

class ResamplerProvider : public IResamplerProvider
{
public:
    ResamplerProvider();
    virtual ~ResamplerProvider();

    virtual void fromFloat(void *output, Type outputType,
                           const float *input, unsigned count) const;

    virtual void initialize(Quality quality, unsigned channels, unsigned frames, double rate);
    virtual float *output();
    virtual void process(const float *input, unsigned inputFramesMax, double inputRate);

    virtual void toFloat(float *output, const void *input, Type inputType,
                         unsigned count) const;

protected:
    unsigned _channels;
    unsigned _frames;
    const float *_input;
    unsigned _inputFramesMax;
    double _inputRate;
    bool _isInitialized;
    float *_output;
    double _outputRate;

    void finalize();
};

#endif // RESAMPLERPROVIDER_H
