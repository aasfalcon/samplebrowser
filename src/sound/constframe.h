#ifndef SOUND_CONSTFRAME_H
#define SOUND_CONSTFRAME_H

#include "frame.h"

namespace Sound {

template <typename T>
class ConstFrame : public Object<T> {
public:
    ConstFrame()
        : _channels(0)
        , _samples(nullptr)
    {
    }

    ConstFrame(unsigned channels, const Sample<T>* data)
        : _channels(channels)
        , _samples(data)
    {
    }

    ConstFrame(const ConstFrame& source)
        : _channels(source._channels)
        , _samples(source._samples)
    {
    }

    ConstFrame(const Frame<T>& source)
        : _channels(source.channels())
        , _samples(source.samples())
    {
    }

    ~ConstFrame() {}

    Sample<T> at(unsigned channel) const
    {
        return _samples[channel];
    }

    unsigned channels() const
    {
        return _channels;
    }

    const T* data() const
    {
        return reinterpret_cast<const T*>(_samples);
    }

    const Sample<T>* samples() const
    {
        return _samples;
    }

    unsigned size() const
    {
        return sizeof(T) * _channels;
    }

    ConstFrame<T>& operator++()
    {
        return operator+=(1);
    }

    ConstFrame<T>& operator--()
    {
        return operator+=(-1);
    }

    int operator-(ConstFrame<T> rht) const
    {
        return (_samples - rht._samples) / int(_channels);
    }

    ConstFrame<T> operator+(int rht) const
    {
        return ConstFrame<T>(_channels, _samples + rht * int(_channels));
    }

    ConstFrame<T> operator-(int rht) const
    {
        return operator+(-rht);
    }

    ConstFrame<T>& operator+=(int rht)
    {
        _samples += (rht * int(_channels));
        return *this;
    }

    ConstFrame<T>& operator-=(int rht)
    {
        return operator+=(-rht);
    }

    bool operator !() const
    {
        return !_samples;
    }

    bool operator==(const ConstFrame<T>& rht) const
    {
        return rht._samples == _samples;
    }

    bool operator!=(const ConstFrame<T>& rht) const
    {
        return rht._samples != _samples;
    }

    Sample<T> operator[](unsigned channel) const
    {
        return at(channel);
    }

private:
    unsigned _channels;
    const Sample<T>* _samples;
};

SOUND_INSTANTIATION_DECLARE(ConstFrame);
}

#endif // SOUND_CONSTFRAME_H
