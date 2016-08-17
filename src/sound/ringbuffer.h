#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include "buffer.h"

template<typename T>
class RingBuffer
{
public:
    RingBuffer();

    bool isEmpty() const;
    bool isFull() const;
    bool isHalfEmpty() const;
    const std::vector<Buffer<T>> &pop();
    void push(const std::vector<Buffer<T>> &data);

private:
    unsigned _begin;
    std::vector<Buffer<T>> _buffers;
    unsigned _end;
};

#endif // RINGBUFFER_H
