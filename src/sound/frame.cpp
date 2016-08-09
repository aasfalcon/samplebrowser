#include <stdexcept>
#include "buffer.h"
#include "constframe.h"
#include "frame.h"
#include "sample.h"

template<typename T>
Frame<T>::Frame(Buffer<T> &buffer)
    : _channels(buffer.channels())
    , _data(buffer.data())
{

}

template<typename T>
Frame<T>::Frame(Buffer<T> &buffer, unsigned offset)
    : _channels(buffer.channels())
    , _data(buffer.data() + offset * buffer.channels())
{

}

template<typename T>
Frame<T>::Frame(const Frame &frame)
    : _channels(frame._channels)
    , _data(frame._data)
{

}

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
{
    return _channels;
}

template<typename T>
void Frame<T>::put(unsigned channel, Sample<T> value)
{
    if (channel >= _channels) {
        throw std::range_error("Channel index out of bounds");
    }

    _data[channel] = value;
}

template<typename T>
Frame<T> &Frame<T>::operator++()
{
    ++_data; return *this;
}

template<typename T>
bool Frame<T>::operator==(const Frame<T> &rht) const
{
    return rht._data == _data;
}

template<typename T>
bool Frame<T>::operator==(const ConstFrame<T> &rht) const
{
    return rht._data == _data;
}

template<typename T>
bool Frame<T>::operator!=(const Frame<T> &rht) const
{
    return rht._data != _data;
}

template<typename T>
bool Frame<T>::operator!=(const ConstFrame<T> &rht) const
{
    return rht._data != _data;
}

SOUND_INSTANTIATE(Frame)
