#include <stdexcept>
#include <string>

#include "constframe.h"
#include "ringbuffer.h"
#include "shared/log.h"

template <typename T>
RingBuffer<T>::RingBuffer(unsigned channels, unsigned frames, unsigned count)
    : _begin(0)
    , _channels(channels)
    , _count(count)
    , _frames(frames)
    , _end(0)
    , _ring(count, Buffer<T>(channels, frames))
{
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
}

template <typename T>
void RingBuffer<T>::clear()
{
    _begin = _end;
}

template <typename T>
bool RingBuffer<T>::isEmpty() const
{
    return loaded() == 0;
}

template <typename T>
bool RingBuffer<T>::isFull() const
{
    return _count == loaded();
}

template <typename T>
bool RingBuffer<T>::isHalfEmpty() const
{
    return _count <= loaded() * 2;
}

template <typename T>
unsigned RingBuffer<T>::loaded() const
{
    return (_end + 1) % _count - _begin - 1;
}

template <typename T>
unsigned RingBuffer<T>::space() const
{
    return _count - loaded();
}

template <typename T>
const Buffer<T>& RingBuffer<T>::pop()
{
    if (isEmpty()) {
        std::string message = "Ring buffer underflow";
        LOG(ERROR, message);
        throw std::out_of_range(message);
    }

    auto& result = _ring[_begin];
    ++_begin %= _count;
    return result;
}

template <typename T>
template <typename S>
void RingBuffer<T>::push(ConstFrame<S> sbeg, ConstFrame<S> send)
{
    if (isFull()) {
        std::string message = "Ring buffer overflow";
        LOG(ERROR, message);
        throw std::out_of_range(message);
    }

    if (send - sbeg > int(_frames)) {
        std::string message = "Attempt to push amount of data larger than ring buffer entry";
        LOG(ERROR, message);
        throw std::out_of_range(message);
    }

    Buffer<T>& dest = _ring.at(_end);
    dest.silence(dest.copy(sbeg, send), dest.end());

    ++_end %= _count;
}

#define RINGBUFFER_PUSH(__ttype, __mtype) \
    template void RingBuffer<__ttype>::push(ConstFrame<__mtype> sbeg, \
                                            ConstFrame<__mtype> send)

SOUND_INSTANTIATE_TEMPLATE_METHOD(RINGBUFFER_PUSH);
SOUND_INSTANTIATE(RingBuffer);
