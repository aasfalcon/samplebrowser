#ifndef SOUND_RINGBUFFER_H
#define SOUND_RINGBUFFER_H

#include "buffer.h"
#include "constframe.h"
#include "shared/ring.h"

namespace Sound {

template <typename T>
class RingBuffer : public Ring<Buffer<T> > {
public:
    RingBuffer(unsigned channels, unsigned frames, unsigned count = 16);
    ~RingBuffer();

    template <typename S>
    void push(ConstFrame<S> sbeg, ConstFrame<S> send)
    {
        Buffer<T>& dest = this->_items[this->_end];
        auto last = dest.copy(sbeg, send);
        dest.silence(last, dest.end());

        ++this->_end %= this->_count;
    }
};

SOUND_INSTANTIATION_DECLARE(RingBuffer);
}

#endif // SOUND_RINGBUFFER_H
