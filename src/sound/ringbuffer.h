#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>

#include "buffer.h"
#include "constframe.h"

namespace Sound {

template <typename T>
class RingBuffer {
public:
    RingBuffer(unsigned channels, unsigned frames, unsigned count = 16);
    ~RingBuffer();

    void clear()
    {
        _begin = _end;
    }

    unsigned count() const
    {
        return _count;
    }

    bool isEmpty() const
    {
        return loaded() == 0;
    }

    bool isFull() const
    {
        return _count == loaded();
    }

    bool isHalfEmpty() const
    {
        return _count <= loaded() * 2;
    }

    unsigned loaded() const
    {
        return (_end + 1) % _count - _begin - 1;
    }

    unsigned space() const
    {
        return _count - loaded();
    }

    const Buffer<T>& pop()
    {
        auto& result = _ring[_begin];

        ++_begin %= _count;
        return result;
    }

    template <typename S>
    void push(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        Buffer<T>& dest = _ring.at(_end);
        dest.silence(dest.copy(sbeg, send), dest.end());

        ++_end %= _count;
    }

private:
    unsigned _begin;
    unsigned _channels;
    unsigned _count;
    unsigned _frames;
    unsigned _end;
    std::vector<Buffer<T> > _ring;
};

SOUND_INSTANTIATION_DECLARE(RingBuffer);
}

#endif // RINGBUFFER_H
