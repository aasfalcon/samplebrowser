#include "error.h"
#include "resampler.h"
#include "resamplerbuffer.h"

namespace Sound {

template<class T>
Resampler<T>::Resampler(ResamplerPrivate::Quality quality)
    : ResamplerPrivate(quality)
{
}

template<class T>
void Resampler<T>::resample(const Buffer<T> &source, int sourceRate,
                            Buffer<T> &dest, int destRate)
{
    _sourceFrames = source.frames();
    _sourceRate = sourceRate;
    _destRate = destRate;
    _destChannels = source.channels();
    _destFrames = ratio() * _sourceFrames;

    ResamplerBuffer<T> convert(source);
    ResamplerBuffer<float> input;
    ResamplerBuffer<float> output(_destFrames, _destChannels);

    convert.toFloat(input);
    process(input.data(), output.data());
    convert.fromFloat(output);
    dest = convert;
}

template class Resampler<short>;
template class Resampler<int>;
template class Resampler<float>;
template class Resampler<double>;

} // namespace Sound
