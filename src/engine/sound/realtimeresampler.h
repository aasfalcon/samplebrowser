#ifndef REALTIMERESAMPLER_H
#define REALTIMERESAMPLER_H

#include "resampler.h"
#include "resamplerbuffer.h"

namespace Sound {

template<class T>
class RealtimeResampler : public Resampler<T>, public ResamplerBuffer<T>
{
public:
    RealtimeResampler(ResamplerPrivate::Quality quality, int outputFrames, int channels, int outputRate);
    int resample(const T *input, int max, int rate);

private:
    ResamplerBuffer<float> _floatInput;
    ResamplerBuffer<float> _floatOutput;
};

} // namespace Sound

#endif // REALTIMERESAMPLER_H
