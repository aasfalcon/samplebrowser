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
Resampler<T>::~Resampler()
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
void Resampler<T>::feed(const Buffer<S> &buffer, unsigned sampleRate)
{
    _sourceMutex.lock();

    _position = 0;
    _source = std::shared_ptr<char>(
                reinterpret_cast<char *>(new S[buffer.size()]),
                [](char *array) { delete[] reinterpret_cast<S *>(array); });
    memcpy(_source.get(), buffer.ptr(), int(buffer.size() * sizeof(S)));
    _sourceChannels = buffer.channels();
    _sourceFrames = buffer.frames();
    _sourceRate = sampleRate;
    _sourceType = buffer.type();

    double ratio =
            static_cast<double>(sampleRate)
            / static_cast<double>(this->sampleRate());

    auto &out = *this->out();
    _bufferFrames = unsigned(ratio * double(out.frames()));
    _buffer.reallocate(buffer.channels(), _bufferFrames);
    _channelBuffer.reallocate(out.channels(), _bufferFrames);
    _resampler.reset();

    _sourceMutex.unlock();
}

template<typename T>
void Resampler<T>::init()
{
    auto &out = *this->out();
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

    auto &out = *this->out();
    Converter::instance().convert(
                out.ptr(), out.type(),
                _resampler->output(), Object<Sound::Float32>::type(),
                resampledFrames);

    if (resampledFrames < out.frames()) {
        unsigned offset = resampledFrames * out.channels();
        std::memset(out.ptr() + offset, 0,
                    std::size_t(out.size() - offset) * sizeof(T));
    }

    _position += resampledFrames;
    _sourceMutex.unlock();
}

#define RESAMPLER_FEED(__typeA, __typeB) \
    template \
    void Resampler<Sound::__typeA>::feed<Sound::__typeB>( \
        const Buffer<Sound::__typeB> &source, unsigned sampleRate);

SOUND_INSTANTIATE(Resampler)
SOUND_SPECIALIZE_COMPLEX(RESAMPLER_FEED)
