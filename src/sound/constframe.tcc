#include "constframe.h"

template<typename T>
ConstFrame<T>::ConstFrame(const Buffer<T> &buffer)
    : _channels(buffer.channels())
    , _data(buffer.data())
{

}

template<typename T>
ConstFrame<T>::ConstFrame(const Buffer<T> &buffer, unsigned offset)
    : _channels(buffer.channels())
    , _data(buffer.data() + offset * buffer.channels())
{

}

template<typename T>
ConstFrame<T>::ConstFrame(const ConstFrame &ConstFrame)
    : _channels(ConstFrame._channels)
    , _data(ConstFrame._data)
{

}

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
{
    return _channels;
}

template<typename T>
ConstFrame<T> &ConstFrame<T>::operator++()
{
    ++_data; return *this;
}

template<typename T>
bool ConstFrame<T>::operator==(const ConstFrame<T> &rht) const
{
    return rht._data == _data;
}

template<typename T>
bool ConstFrame<T>::operator==(const Frame<T> &rht) const
{
    return rht._data == _data;
}

template<typename T>
bool ConstFrame<T>::operator!=(const ConstFrame<T> &rht) const
{
    return rht._data != _data;
}

template<typename T>
bool ConstFrame<T>::operator!=(const Frame<T> &rht) const
{
    return rht._data != _data;
}
