#ifndef SOUND_BUFFER_H
#define SOUND_BUFFER_H

#include <vector>

#include "constframe.h"
#include "frame.h"
#include "shared/iresampler.h"
#include "shared/log.h"

namespace Sound {

template <typename T>
class Buffer : public Object<T> {
public:
    Buffer()
        : _channels(0)
        , _frames(0)
    {
    }

    Buffer(const Buffer<T>& that)
        : _channels(that._channels)
        , _frames(that._frames)
        , _samples(that._samples)
    {
    }

    Buffer(unsigned channels, unsigned frames)
        : _channels(channels)
        , _frames(frames)
        , _samples(channels * frames)
    {
        ERROR_IF(!channels || !frames, RUNTIME_ERROR,
            "Invalid buffer constructor parameters");
    }

    template <typename S>
    Buffer(const Buffer<S>& that)
        : Buffer(that.cbegin(), that.cend())
    {
    }

    template <typename S>
    Buffer(ConstFrame<S> sbeg, ConstFrame<S> send)
        : Buffer<T>(sbeg.channels(), send - sbeg)
    {
        copyFrom(sbeg, send);
    }

    template <typename S>
    Buffer(unsigned channels, unsigned frames, const Sample<S>* psample)
        : Buffer<T>(channels, frames)
    {
        auto sbeg = ConstFrame<S>(channels, psample);
        copyFrom(sbeg, sbeg + frames);
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
        copyFrom(sbeg, send);
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
    Frame<T> copyFrom(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        return copy(begin(), end(), sbeg, send);
    }

    template <typename D>
    Frame<D> copyTo(Frame<D> dbeg, Frame<D> dend)
    {
        return copy(dbeg, dend, cbegin(), cend());
    }

    Sample<T>* data()
    {
        return _samples.data();
    }

    const Sample<T>* data() const
    {
        return _samples.data();
    }

    Frame<T> end()
    {
        return begin() + _frames;
    }

    unsigned frames() const
    {
        return _frames;
    }

    void fromInt24(const void* source);

    bool isEmpty() const
    {
        return _frames == 0;
    }

    void toInt24(void* dest) const;
    void reallocate(unsigned channels, unsigned frames);

    void reshape(unsigned channels, unsigned frames)
    {
        ERROR_IF(channels * frames != _channels * _frames, RUNTIME_ERROR,
            "Invalid buffer reshaping options");
        _channels = channels;
        _frames = frames;
    }

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

    static void silence(Frame<T> dbeg, Frame<T> dend);

    unsigned size() const
    {
        return _channels * _frames;
    }

private:
    unsigned _channels;
    unsigned _frames;
    std::vector<Sample<T> > _samples;

    template <typename S>
    static Frame<T> copy(Frame<T> dit, Frame<T> dend,
        ConstFrame<S> sit, ConstFrame<S> send);
};

SOUND_INSTANTIATION_DECLARE(Buffer);
}

#include "buffer.tcc"

#endif // SOUND_BUFFER_H
