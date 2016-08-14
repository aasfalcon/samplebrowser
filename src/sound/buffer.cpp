#include <cstring>

#include "buffer.h"
#include "constframe.h"
#include "converter.h"
#include "frame.h"
#include "sample.h"

template<typename T>
Buffer<T>::Buffer()
    : _channels(0)
    , _frames(0)
    , _samples()
{

}

template<typename T>
Buffer<T>::Buffer(const Buffer<T> &source)
    : _channels(source._channels)
    , _frames(source._frames)
    , _samples(source._samples)
{

}

template<typename T>
Buffer<T>::Buffer(unsigned channels, unsigned frames)
    : _channels(channels)
    , _frames(frames)
    , _samples(channels * frames, static_cast<T>(0))
{

}

template<typename T>
Frame<T> Buffer<T>::begin()
{
    return Frame<T>(*this);
}

template<typename T>
ConstFrame<T> Buffer<T>::cbegin() const
{
    return ConstFrame<T>(*this);
}

template<typename T>
ConstFrame<T> Buffer<T>::cend() const
{
    return ConstFrame<T>(*this, size());
}

template<typename T>
unsigned Buffer<T>::channels() const
{
    return _channels;
}

template<typename T>
Sample<T> *Buffer<T>::data()
{
    return _samples.data();
}

template<typename T>
const Sample<T> *Buffer<T>::data() const
{
    return _samples.data();
}

template<typename T>
Frame<T> Buffer<T>::end()
{
    return Frame<T>(*this, size());
}

template<typename T>
unsigned Buffer<T>::frames() const
{
    return _frames;
}

template<typename T>
template<typename S>
void Buffer<T>::fromArray(const Sample<S> *array,
                          unsigned channels, unsigned frames)
{
    fromArray(array, Sample<S>::type(), channels, frames);
}

template<typename T>
void Buffer<T>::fromArray(const void *array, Sound::Type type,
                          unsigned channels, unsigned frames)
{
    reallocate(channels, frames);
    Converter::instance().convert(
                data(), Sample<T>::type(), array, type, size());
}

template<typename T>
bool Buffer<T>::isEmpty() const
{
    return _frames == 0;
}

template<typename T>
template<typename S>
void Buffer<T>::fromBuffer(const Buffer<S> &source,
                           unsigned offset, unsigned count)
{
    unsigned channels = source.channels();
    unsigned frames = source.frames() - offset;

    if (count && count < frames) {
        frames = count;
    }

    fromArray(source.data() + offset * channels * frames,
              channels, frames);
}

template<typename T>
T *Buffer<T>::ptr()
{
    return reinterpret_cast<T *>(_samples.data());
}

template<typename T>
const T *Buffer<T>::ptr() const
{
    return reinterpret_cast<const T *>(_samples.data());
}

template<typename T>
void Buffer<T>::reallocate(unsigned channels, unsigned frames)
{
    if (_channels != _channels && _frames != frames) {
        if (_channels * _frames < channels * frames) {
            _samples.clear();
        }

        _samples.resize(channels * frames);
        _channels = channels;
        _frames = frames;
    }
}

template<typename T>
void Buffer<T>::resize(unsigned frames)
{
    if (_frames != frames) {
        _samples.resize(_channels * frames);
        _frames = frames;
    }
}

template<typename T>
Buffer<T> Buffer<T>::resample(unsigned destRate, unsigned sourceRate,
                              Sound::Quality quality)
{
    Buffer<T> result;
    result.resample(*this, destRate, sourceRate, quality);
    return result;
}

template<typename T>
void Buffer<T>::resample(const Buffer<T> &source, unsigned destRate, unsigned sourceRate,
                      Sound::Quality quality)
{
    double ratio = static_cast<double>(destRate) / static_cast<double>(sourceRate);
    unsigned sourceFrames = source.frames();
    unsigned channels = source.channels();
    unsigned destFrames = unsigned(double(sourceFrames) * ratio);

    reallocate(channels, destFrames);
    Converter::instance().resample(
                data(), destFrames, source.data(), sourceFrames,
                channels, ratio, Sample<T>::type(), quality);
}

template<typename T>
void Buffer<T>::peekChannels(const Buffer<T> &source,
                             unsigned count, bool mixdown)
{
    if (count == 0) {
        throw std::range_error("Zero channlels slice required");
    }

    if (source._channels == count) {
        this->fromBuffer(source);
    }

    if (1 == count) {
        mono(source, mixdown);
    }

    if (2 == count) {
        stereo(source, mixdown);
    }

    reallocate(count, frames());
    silence();

    unsigned copyCount = std::min(_channels, count);
    auto rframe = begin();

    for (auto frame = source.cbegin(); frame != source.cend();
         ++frame, ++rframe)
    {
        for (unsigned i = 0; i < copyCount; i++) {
            rframe.put(i, frame.at(i));
        }
    }
}

template<typename T>
void Buffer<T>::mono(const Buffer<T> &source, bool mixdown)
{
    if (1 == _channels) {
        fromBuffer(source);
    }

    reallocate(1, frames());
    auto rframe = begin();

    if (mixdown) {
        for (auto frame = source.cbegin(); frame != source.cend();
             ++frame, ++rframe)
        {
            double sum = 0.;

            for (unsigned i = 0; i < _channels; i++) {
                sum += (double)(T)frame.at(i);
            }

            rframe.put(0, static_cast<T>(sum / _channels));
        }
    } else {
        for (auto frame = source.cbegin(); frame != source.cend();
             ++frame, ++rframe)
        {
            rframe.put(0, frame.at(0));
        }
    }
}

template<typename T>
void Buffer<T>::silence()
{
    memset((void *)data(), 0, size() * sizeof(T));
}

template<typename T>
unsigned Buffer<T>::size() const
{
    return _samples.size();
}

template<typename T>
void Buffer<T>::stereo(const Buffer<T> &source, bool pullup)
{
    if (2 == _channels) {
        fromBuffer(source);
    }

    reallocate(2, frames());
    auto rframe = begin();

    if (1 == _channels && pullup) {
        for (auto frame = source.cbegin(); frame != source.cend();
             ++frame, ++rframe)
        {
            auto sample = frame.at(0);
            rframe.put(0, sample);
            rframe.put(1, sample);
        }
    } else { // > 2
        for (auto frame = source.cbegin(); frame != source.cend();
             ++frame, ++rframe)
        {
            rframe.put(0, frame.at(0));
            rframe.put(1, frame.at(1));
        }
    }
}

SOUND_INSTANTIATE(Buffer)
