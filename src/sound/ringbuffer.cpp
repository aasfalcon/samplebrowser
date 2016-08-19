#include <cassert>
#include <stdexcept>
#include <string>

#include "constframe.h"
#include "ringbuffer.h"
#include "shared/log.h"

using namespace Sound;

template <typename T>
RingBuffer<T>::RingBuffer(unsigned channels, unsigned frames, unsigned count)
    : _begin(0)
    , _channels(channels)
    , _count(count
             )
    , _frames(frames)
    , _end(0)
    , _ring(_count, Buffer<T>(channels, frames))
{
    assert(count > 1 && channels && frames);
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
}

SOUND_INSTANTIATE(RingBuffer);
