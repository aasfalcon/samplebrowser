#ifndef BUFFER_H
#define BUFFER_H

#include <cstring>
#include <stdexcept>
#include <vector>

#include "object.h"
#include "sample.h"
#include "shared/iresampler.h"

namespace Sound {

template <typename T>
class Frame;

template <typename T>
class ConstFrame;

template <typename T>
class Buffer : public Object<T> {
public:
    Buffer()
        : _channels(0)
        , _frames(0)
        , _samples()
    {
    }

    Buffer(unsigned channels, unsigned frames)
        : _channels(channels)
        , _frames(frames)
        , _samples(channels * frames)
    {
    }

    template <typename S>
    Buffer(ConstFrame<S> sbeg, ConstFrame<S> send)
        : Buffer<T>(sbeg.channels(), send - sbeg)
    {
        copy(sbeg, send);
    }

    template <typename S>
    Buffer(unsigned channels, unsigned frames, const Sample<S>* psample)
        : Buffer<T>(channels, frames)
    {
        auto sbeg = ConstFrame<S>(channels, psample);
        assign(sbeg, sbeg + frames);
    }

    template <typename S>
    Buffer(unsigned channels, unsigned frames, const S* ptr)
        : Buffer<T>(channels, frames, reinterpret_cast<const Sample<S>*>(ptr))
    {
    }

    template <typename S>
    void assign(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        reallocate(sbeg.channels(), send - sbeg);
        copy(sbeg, send);
    }

    Frame<T> begin()
    {
        return Frame<T>(_channels, _samples.data());
    }

    ConstFrame<T> cbegin() const
    {
        return ConstFrame<T>(_channels, _samples.data());
    }

    ConstFrame<T> cend() const
    {
        return cbegin() + _frames;
    }

    unsigned channels() const
    {
        return _channels;
    }

    template <typename S>
    Frame<T> copy(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        int count = send - sbeg;

        if (_frames < unsigned(count)) {
            throw std::out_of_range("Buffer overflow");
        }

        Frame<T> dit = begin();

        if (this->type() == sbeg.type() && _channels == sbeg.channels()) {
            std::memcpy(this->data(), sbeg.data(), count * _channels * sizeof(T));
            dit += count;
        } else {
            ConstFrame<S> sit = sbeg;

            while (sit != send) {
                dit = sit;
                ++dit;
                ++sit;
            }
        }

        return dit;
    }

    Frame<T> end()
    {
        return begin() + _frames;
    }

    unsigned frames() const
    {
        return _frames;
    }

    bool isEmpty() const
    {
        return _frames == 0;
    }

    Sample<T>* data()
    {
        return _samples.data();
    }

    const Sample<T>* data() const
    {
        return _samples.data();
    }

    void toInt24(void* dest);
    void reallocate(unsigned channels, unsigned frames);
    Buffer<T> resample(unsigned destRate, unsigned sourceRate,
        IResampler::Quality quality = IResampler::QualityBest);
    void resize(unsigned frames);
    void resample(ConstFrame<T> sbeg, ConstFrame<T> send,
        unsigned destRate, unsigned sourceRate,
        IResampler::Quality quality = IResampler::QualityBest);

    void silence()
    {
        silence(begin(), end());
    }

    void silence(Frame<T> dbeg, Frame<T> dend);

    unsigned size() const
    {
        return _samples.size();
    }

private:
    unsigned _channels;
    unsigned _frames;
    std::vector<Sample<T> > _samples;
};

SOUND_INSTANTIATION_DECLARE(Buffer);
}

#endif // BUFFER_H
