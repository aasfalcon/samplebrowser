#include <stdexcept>

#include "frame.h"

template<typename T>
Frame<T>::Frame(Buffer<T> &buffer, unsigned offset)
    : _channels(buffer.channels())
    , _data(buffer.data() + offset * _channels)
{}

template<typename T>
Frame<T>::Frame(unsigned channels, Sample<T> *ptr)
    : _channels(channels)
    , _data(ptr)
{}

template<typename T>
Frame<T>::Frame(const Frame &frame)
    : _channels(frame._channels)
    , _data(frame._data)
{}

template<typename T>
Sample<T> Frame<T>::at(unsigned channel) const
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    return _data[channel];
}

template<typename T>
unsigned Frame<T>::channels() const
{ return _channels; }

template<typename T>
const Sample<T> *Frame<T>::data() const
{ return _data; }

template<typename T>
Sample<T> *Frame<T>::data()
{ return _data; }

template<typename T>
void Frame<T>::put(unsigned channel, Sample<T> value)
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    _data[channel] = value;
}

template<typename T>
unsigned Frame<T>::size() const
{ return sizeof(T) * _channels; }

template<typename T>
Frame<T> &Frame<T>::operator++()
{ return operator +=(1); }

template<typename T>
Frame<T> &Frame<T>::operator--()
{ return operator +=(-1); }

template<typename T>
int Frame<T>::operator -(Frame<T> rht) const
{ return (_data - rht._data) / _channels; }

template<typename T>
Frame<T> Frame<T>::operator +(int rht) const
{ return Frame<T>(_data + rht * _channels); }

template<typename T>
Frame<T> Frame<T>::operator -(int rht) const
{ return operator +(-rht); }

template<typename T>
Frame<T> &Frame<T>::operator +=(int rht)
{
    _data += (rht * _channels);
    return *this;
}

template<typename T>
Frame<T> &Frame<T>::operator -=(int rht)
{ return operator +=(-rht); }

template<typename T>
bool Frame<T>::operator==(const ConstFrame<T> &rht) const
{ return rht._data == _data; }

template<typename T>
bool Frame<T>::operator!=(const ConstFrame<T> &rht) const
{ return rht._data != _data; }

template<typename T>
Sample<T> Frame<T>::operator [](unsigned channel) const
{ return at(channel); }

template<typename T>
Sample<T> &Frame<T>::operator [](unsigned channel)
{ return _data[channel]; }

template<typename T>
template<typename S>
Frame<T> &Frame<T>::operator =(ConstFrame<S> rht)
{
    if (_channels <= rht._channels) {
        for (unsigned i = 0; i < _channels; i++) {
            put(i, rht->at(i));
        }
    } else if (_channels == 2) {
        auto sample = rht.at(0);
        put(0, sample);
        put(1, sample);
    } else {
        for (unsigned i = 0; i < rht._channels; i++) {
            put(i, rht.at(i));
        }

        for (unsigned i = rht->_channels; i < _channels; i++) {
            put(i, 0);
        }
    }

    return *this;
}
