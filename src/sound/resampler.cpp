#include <cstring>
#include "converter.h"
#include "resampler.h"
#include "sample.h"
#include "shared/iresampler.h"
#include "shared/server.h"

template<typename T>
Resampler<T>::Resampler()
{

}

template<typename T>
void Resampler<T>::drop()
{
    _sourceMutex.lock();
    _source.reset();
    _sourceMutex.unlock();
}

template<typename T>
template<typename S>
void Resampler<T>::feed(const Buffer<S> &source, unsigned sampleRate)
{
    _sourceMutex.lock();

    _position = 0;
    _source = new S[source.size()];
    memcpy(_source.get(), source.data(),
           source.size() * sizeof(S));
    _sourceChannels = source.channels();
    _sourceFrames = source.frames();
    _sourceRate = sampleRate;
    _sourceType = source.type();

    double ratio =
            static_cast<double>(sampleRate)
            / static_cast<double>(this->sampleRate());

    _bufferFrames = ratio * this->out().frames();
    _buffer.reallocate(source.channels(), _bufferFrames);
    _channelBuffer.reallocate(this->out().channels(), _bufferFrames);
    _resampler.reset();

    _sourceMutex.unlock();
}

template<typename T>
void Resampler<T>::init()
{
    Buffer<T> &out = this->out();
    _resampler = PLUGIN_FACTORY(IResampler);
    _resampler->init(Sound::QualityRealtime, out.channels(),
                     out.frames(), this->sampleRate());
}

template<typename T>
void Resampler<T>::process()
{
    if (!_sourceMutex.try_lock()) {
        return;
    }

    if (!_source || _position >= _sourceFrames) {
        _sourceMutex.unlock();
        return;
    }

    _buffer.fromArray(_source.get() + _position * _sourceChannels,
                      _sourceType, _sourceChannels,
                      std::min(_bufferFrames, _sourceFrames - _position));
    _channelBuffer.peekChannels(_buffer, _channelBuffer.channels());

    unsigned resampledFrames =
            _resampler->process(_channelBuffer.ptr(), _bufferFrames);

    Buffer<T> &out = this->out();
    Converter::convert(out.ptr(), out.type(),
                       _resampler->output(), Object<Sound::Float32>::type(),
                       resampledFrames);

    if (resampledFrames < out.frames()) {
        unsigned offset = resampledFrames * out.channels();
        memset(out.ptr() + offset, 0, (out.size() - offset) * sizeof(T));
    }

    _position += resampledFrames;
    _sourceMutex.unlock();
}

SOUND_INSTANTIATE(Resampler)
