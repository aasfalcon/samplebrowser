#ifndef SOUND_FRAME_H
#define SOUND_FRAME_H

#include <cstring>

#include "object.h"
#include "sample.h"

namespace Sound {

template <typename T>
class ConstFrame;

template <typename T>
class Frame : public Object<T> {
public:
    Frame()
        : _channels(0)
        , _samples(nullptr)
    {
    }

    Frame(unsigned channels, Sample<T>* samples)
        : _channels(channels)
        , _samples(samples)
    {
    }

    Frame(unsigned channels, T* ptr)
        : _channels(channels)
        , _samples(reinterpret_cast<Sample<T>*>(ptr))
    {
    }

    Frame(const Frame& source)
        : _channels(source._channels)
        , _samples(source._samples)
    {
    }

    ~Frame() {}

    const Sample<T>& at(unsigned channel) const
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

    T* data()
    {
        return reinterpret_cast<T*>(_samples);
    }

    const Sample<T>* samples() const
    {
        return _samples;
    }

    Sample<T>* samples()
    {
        return _samples;
    }

    unsigned size() const
    {
        return sizeof(T) * _channels;
    }

    Frame<T>& operator++()
    {
        return operator+=(1);
    }

    Frame<T>& operator--()
    {
        return operator+=(-1);
    }

    int operator-(Frame<T> rht) const
    {
        return (_samples - rht._samples) / int(_channels);
    }

    Frame<T> operator+(int rht) const
    {
        return Frame<T>(_channels, _samples + rht * int(_channels));
    }

    Frame<T>& operator+=(int rht)
    {
        _samples += (rht * int(_channels));
        return *this;
    }

    Frame<T> operator-(int rht) const
    {
        return operator+(-rht);
    }

    Frame<T>& operator-=(int rht)
    {
        return operator+=(-rht);
    }

    bool operator==(const ConstFrame<T>& rht) const
    {
        return rht.samples() == _samples;
    }

    bool operator!=(const ConstFrame<T>& rht) const
    {
        return !operator==(rht);
    }

    const Sample<T>& operator[](unsigned channel) const
    {
        return _samples[channel];
    }

    Sample<T>& operator[](unsigned channel)
    {
        return _samples[channel];
    }

    void put(unsigned channel, Sample<T> value)
    {
        _samples[channel] = value;
    }

    template <typename S>
    void put(Frame<S> source)
    {
        put(ConstFrame<S>(source));
    }

    template <typename S>
    void put(ConstFrame<S> source)
    {
        if (1 == _channels && 1 < source.channels()) {
            Sample<Precise> left = source[0];
            Sample<Precise> right = source[1];
            _samples[0] = Sample<Precise>((left.value() + right.value()) / 2);
        } else if (1 == source.channels() && 1 < _channels) {
            _samples[0] = _samples[1] = source[0];
        } else {
            unsigned c = 0;

            for (; c < std::min(_channels, source.channels()); c++) {
                _samples[c] = source[c];
            }

            for (; c < _channels; c++) {
                _samples[c] = this->nil();
            }
        }
    }

private:
    unsigned _channels;
    Sample<T>* _samples;
};

SOUND_INSTANTIATION_DECLARE(Frame);
}

#endif // SOUND_FRAME_H
