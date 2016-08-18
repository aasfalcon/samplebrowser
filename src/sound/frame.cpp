#include <stdexcept>

#include "constframe.h"
#include "frame.h"

template <typename T>
Frame<T>::Frame(unsigned channels, Sample<T>* ptr)
    : _channels(channels)
    , _data(ptr)
{
}

template <typename T>
Frame<T>::Frame(const Frame& frame)
    : _channels(frame._channels)
    , _data(frame._data)
{
}

template <typename T>
Sample<T> Frame<T>::at(unsigned channel) const
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    return _data[channel];
}

template <typename T>
unsigned Frame<T>::channels() const
{
    return _channels;
}

template <typename T>
const Sample<T>* Frame<T>::data() const
{
    return _data;
}

template <typename T>
Sample<T>* Frame<T>::data()
{
    return _data;
}

template <typename T>
T* Frame<T>::ptr()
{
    return reinterpret_cast<T*>(_data);
}

template <typename T>
const T* Frame<T>::ptr() const
{
    return reinterpret_cast<const T*>(_data);
}

template <typename T>
void Frame<T>::put(unsigned channel, Sample<T> value)
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    _data[channel] = value;
}

template <typename T>
unsigned Frame<T>::size() const
{
    return sizeof(T) * _channels;
}

template <typename T>
Frame<T>& Frame<T>::operator++()
{
    return operator+=(1);
}

template <typename T>
Frame<T>& Frame<T>::operator--()
{
    return operator+=(-1);
}

template <typename T>
int Frame<T>::operator-(Frame<T> rht) const
{
    return (_data - rht._data) / int(_channels);
}

template <typename T>
Frame<T> Frame<T>::operator+(int rht) const
{
    return Frame<T>(_channels, _data + rht * int(_channels));
}

template <typename T>
Frame<T> Frame<T>::operator-(int rht) const
{
    return operator+(-rht);
}

template <typename T>
Frame<T>& Frame<T>::operator+=(int rht)
{
    _data += (rht * int(_channels));
    return *this;
}

template <typename T>
Frame<T>& Frame<T>::operator-=(int rht)
{
    return operator+=(-rht);
}

template <typename T>
bool Frame<T>::operator==(const ConstFrame<T>& rht) const
{
    return rht.data() == _data;
}

template <typename T>
bool Frame<T>::operator!=(const ConstFrame<T>& rht) const
{
    return rht.data() != _data;
}

template <typename T>
Sample<T> Frame<T>::operator[](unsigned channel) const
{
    return at(channel);
}

template <typename T>
Sample<T>& Frame<T>::operator[](unsigned channel)
{
    return _data[channel];
}

template <typename T>
template <typename S>
Frame<T>& Frame<T>::operator=(ConstFrame<S> rht)
{
    unsigned rchannels = rht.channels();

    if (_channels <= rchannels) {
        for (unsigned i = 0; i < _channels; i++) {
            _data[i] = rht[i];
        }
    } else if (_channels == 2) {
        auto sample = rht.at(0);
        _data[0] = sample;
        _data[1] = sample;
    } else {
        for (unsigned i = 0; i < rchannels; i++) {
            _data[i] = rht[i];
        }

        for (unsigned i = rchannels; i < _channels; i++) {
            _data[i] = 0;
        }
    }

    return *this;
}

#define FRAME_ASSIGN(__ttype, __mtype) \
    template Frame<__ttype>& Frame<__ttype>::operator=<__mtype>(ConstFrame<__mtype> rht)

SOUND_INSTANTIATE_TEMPLATE_METHOD(FRAME_ASSIGN);
SOUND_INSTANTIATE(Frame);
