#ifndef INTERFACE_H
#define INTERFACE_H

#include "buffer.h"
#include "plugins/iresamplerprovider.h"

namespace Sound {
class ResamplerPrivate {
protected:
    ResamplerPrivate(IResamplerProvider *provider);
    ~ResamplerPrivate();

    inline void convert(void *output, IResamplerProvider::Type outputType,
                         const void *input, IResamplerProvider::Type inputType, unsigned count) const {
        _provider->convert(output, outputType, input, inputType, count); }
    inline void initialize(IResamplerProvider::Quality quality, unsigned channels, unsigned frames, double rate) {
        _provider->initialize(quality, channels, frames, rate); }
    inline void process(const Buffer<float> &input, double rate) {
        _provider->process(input.data(), input.frames(), rate); }

private:
    IResamplerProvider *_provider;
};

template <class T>
class Resampler : private ResamplerPrivate, public Buffer<T> {
public:
    Resampler(IResamplerProvider *provider);

    inline void initialize(IResamplerProvider::Quality quality, unsigned channels, unsigned frames, double rate)
    {
        ResamplerPrivate::initialize(quality, channels, frames, rate);
        (*this) = Buffer<T>(frames, channels);
        _inputBuffer = Buffer<T>(frames * 8, channels);
    }

    inline void process(const Buffer<T> &input, double rate, unsigned offset = 0)
    {
        unsigned frames = input.frames() - offset;
        unsigned framesOffset = input.channels() * offset;

        _inputBuffer.framesResize(frames);
        toFloat(_inputBuffer.data(), input.data() + framesOffset, frames);
        ResamplerPrivate::process(_inputBuffer, rate);
        fromFloat(this->data(), _provider->output(), this->size());
    }

private:
    Buffer<float> _inputBuffer;

    void fromFloat(T *output, const float *input, unsigned count) const;
    void toFloat(float *output, const T *input, unsigned count) const;
};

} // namespace Sound

#endif // INTERFACE_H
