#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <vector>

#include "buffer.h"
#include "constframe.h"

template<typename T>
class RingBuffer
{
public:
    RingBuffer(unsigned channels, unsigned frames, unsigned count = 16);
    ~RingBuffer();

    void clear();
    unsigned count() const;
    bool isEmpty() const;
    bool isFull() const;
    bool isHalfEmpty() const;
    unsigned loaded() const;
    unsigned space() const;

    const Buffer<T> &pop();

    template<typename S>
    void push(ConstFrame<S> sbeg, ConstFrame<S> send);

private:
    unsigned _begin;
    unsigned _channels;
    unsigned _count;
    unsigned _frames;
    unsigned _end;
    std::vector<Buffer<T>> _ring;
};

#include "ringbuffer.tcc"

#endif // RINGBUFFER_H
