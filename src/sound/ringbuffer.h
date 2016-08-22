#ifndef SOUND_RINGBUFFER_H
#define SOUND_RINGBUFFER_H

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
        return loaded() == _count - 1;
    }

    bool isHalfFull() const
    {
        return loaded() >= _count / 2;
    }

    unsigned loaded() const
    {
        return (_count - _begin + _end) % _count;
    }

    unsigned space() const
    {
        return _count - 1 - loaded();
    }

    const Buffer<T>& pop()
    {
        auto& result = _buffers[_begin];

        ++_begin %= _count;
        return result;
    }

    template <typename S>
    void push(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        Buffer<T>& dest = _buffers.at(_end);
        auto last = dest.copy(sbeg, send);
        dest.silence(last, dest.end());

        ++_end %= _count;
    }

private:
    unsigned _begin;
    std::vector<Buffer<T> > _buffers;
    unsigned _channels;
    unsigned _count;
    unsigned _frames;
    unsigned _end;
};

SOUND_INSTANTIATION_DECLARE(RingBuffer);
}

#endif // SOUND_RINGBUFFER_H
