#include "ringbuffer.h"

template<typename T>
RingBuffer<T>::RingBuffer(unsigned channels, unsigned frames, unsigned count)
{
    for (unsigned i = 0; i < count; i++) {
        _buffers.push_back(Buffer<T>(channels, frames));
    }

    _begin = _end = 0;
}

template<typename T>
bool RingBuffer<T>::isEmpty() const
{
    return _begin == _end;
}
