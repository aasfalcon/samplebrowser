#ifndef CONSTFRAME_H
#define CONSTFRAME_H

#include "frame.h"

namespace Sound {

template <typename T>
class ConstFrame : public Object<T> {
public:
    ConstFrame(unsigned channels, const Sample<T>* data)
        : _channels(channels)
        , _data(data)
    {
    }

    ConstFrame(const ConstFrame& source)
        : _channels(source._channels)
        , _data(source._data)
    {
    }

    ConstFrame(const Frame<T>& source)
        : _channels(source.channels())
        , _data(source.data())
    {
    }

    ~ConstFrame() {}

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

    const T* ptr() const
    {
        return reinterpret_cast<const T*>(_data);
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
        return (_data - rht._data) / int(_channels);
    }

    ConstFrame<T> operator+(int rht) const
    {
        return ConstFrame<T>(_channels, _data + rht * int(_channels));
    }

    ConstFrame<T> operator-(int rht) const
    {
        return operator+(-rht);
    }

    ConstFrame<T>& operator+=(int rht)
    {
        _data += (rht * int(_channels));
        return *this;
    }

    ConstFrame<T>& operator-=(int rht)
    {
        return operator+=(-rht);
    }

    bool operator==(const ConstFrame<T>& rht) const
    {
        return rht._data == _data;
    }

    bool operator!=(const ConstFrame<T>& rht) const
    {
        return rht._data != _data;
    }

    Sample<T> operator[](unsigned channel) const
    {
        return at(channel);
    }

private:
    unsigned _channels;
    const Sample<T>* _data;
};

SOUND_INSTANTIATION_DECLARE(ConstFrame);
}

#endif // CONSTFRAME_H
