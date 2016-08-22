#ifndef SOUND_FRAME_H
#define SOUND_FRAME_H

#include "object.h"
#include "sample.h"

namespace Sound {

template <typename T>
class ConstFrame;

template <typename T>
class Frame : public Object<T> {
public:
    Frame(unsigned channels, Sample<T>* ptr)
        : _channels(channels)
        , _data(ptr)
    {
    }

    Frame(const Frame& source)
        : _channels(source._channels)
        , _data(source._data)
    {
    }

    ~Frame() {}

    Sample<T> at(unsigned channel) const
    {
        return _data[channel];
    }

    unsigned channels() const
    {
        return _channels;
    }

    const Sample<T>* data() const
    {
        return _data;
    }

    Sample<T>* data()
    {
        return _data;
    }

    template <typename S>
    void mix(ConstFrame<S> rht, double level)
    {
        unsigned rchannels = rht.channels();

        if (_channels <= rchannels) {
            for (unsigned i = 0; i < _channels; i++) {
                _data[i] = rht[i] * level;
            }
        } else if (_channels == 2) {
            _data[1] = _data[0] = rht[0] * level;
        } else {
            unsigned i = 0;

            while (i < rchannels) {
                _data[i] = rht[i] * level;
                ++i;
            }

            while(i < _channels) {
                _data[i++] = 0;
            }
        }
    }

    const T* ptr() const
    {
        return reinterpret_cast<const T*>(_data);
    }

    T* ptr()
    {
        return reinterpret_cast<T*>(_data);
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
        return (_data - rht._data) / int(_channels);
    }

    Frame<T> operator+(int rht) const
    {
        return Frame<T>(_channels, _data + rht * int(_channels));
    }

    Frame<T> operator-(int rht) const
    {
        return operator+(-rht);
    }

    Frame<T>& operator+=(int rht)
    {
        _data += (rht * int(_channels));
        return *this;
    }

    Frame<T>& operator-=(int rht)
    {
        return operator+=(-rht);
    }

    bool operator==(const ConstFrame<T>& rht) const
    {
        return rht.data() == _data;
    }

    bool operator!=(const ConstFrame<T>& rht) const
    {
        return rht.data() != _data;
    }

    Sample<T> operator[](unsigned channel) const
    {
        return at(channel);
    }

    Sample<T>& operator[](unsigned channel)
    {
        return _data[channel];
    }

    void put(unsigned channel, Sample<T> value)
    {
        _data[channel] = value;
    }

    template <typename S>
    Frame<T>& operator=(ConstFrame<S> rht)
    {
        unsigned rchannels = rht.channels();

        if (_channels <= rchannels) {
            for (unsigned i = 0; i < _channels; i++) {
                _data[i] = rht[i];
            }
        } else if (_channels == 2) {
            _data[1] = _data[0] = rht[0];
        } else {
            unsigned i = 0;

            while (i < rchannels) {
                _data[i] = rht[i];
                ++i;
            }

            while(i < _channels) {
                _data[i++] = 0;
            }
        }

        return *this;
    }

private:
    unsigned _channels;
    Sample<T>* _data;
};

SOUND_INSTANTIATION_DECLARE(Frame);
}

#endif // SOUND_FRAME_H
