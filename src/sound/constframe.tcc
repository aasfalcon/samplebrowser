#include "constframe.h"

template<typename T>
ConstFrame<T>::ConstFrame(const Buffer<T> &buffer, unsigned offset)
    : _channels(buffer.channels())
    , _data(buffer.data() + offset * _channels)
{}

template<typename T>
ConstFrame<T>::ConstFrame(unsigned channels, const Sample<T> *ptr)
    : _channels(channels)
    , _data(ptr)
{}

template<typename T>
ConstFrame<T>::ConstFrame(const ConstFrame &ConstFrame)
    : _channels(ConstFrame._channels)
    , _data(ConstFrame._data)
{}

template<typename T>
ConstFrame<T>::ConstFrame(const Frame<T> &source)
    : _channels(source.channels())
    , _data(source.data())
{}

template<typename T>
Sample<T> ConstFrame<T>::at(unsigned channel) const
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    return _data[channel];
}

template<typename T>
unsigned ConstFrame<T>::channels() const
{ return _channels; }

template<typename T>
const Sample<T> *ConstFrame<T>::data() const
{ return _data; }

template<typename T>
unsigned ConstFrame<T>::size() const
{ return sizeof(T) * _channels; }

template<typename T>
ConstFrame<T> &ConstFrame<T>::operator++()
{ return operator +=(1); }

template<typename T>
ConstFrame<T> &ConstFrame<T>::operator--()
{ return operator +=(-1); }

template<typename T>
int ConstFrame<T>::operator -(ConstFrame<T> rht) const
{ return (_data - rht._data) / _channels; }

template<typename T>
ConstFrame<T> ConstFrame<T>::operator +(int rht) const
{ return ConstFrame<T>(_data + rht * _channels); }

template<typename T>
ConstFrame<T> ConstFrame<T>::operator -(int rht) const
{ return operator +(-rht); }

template<typename T>
ConstFrame<T> &ConstFrame<T>::operator +=(int rht)
{
    _data += (rht * _channels);
    return *this;
}

template<typename T>
ConstFrame<T> &ConstFrame<T>::operator -=(int rht)
{ return operator +=(-rht); }

template<typename T>
bool ConstFrame<T>::operator==(const ConstFrame<T> &rht) const
{ return rht._data == _data; }

template<typename T>
bool ConstFrame<T>::operator!=(const ConstFrame<T> &rht) const
{ return rht._data != _data; }

template<typename T>
Sample<T> ConstFrame<T>::operator [](unsigned channel) const
{ return at(channel); }
