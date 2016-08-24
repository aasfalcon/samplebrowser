#include <cassert>
#include <stdexcept>
#include <string>

#include "constframe.h"
#include "ringbuffer.h"
#include "shared/log.h"

using namespace Sound;

template <typename T>
RingBuffer<T>::RingBuffer(unsigned channels, unsigned frames, unsigned count)
    : Ring<Buffer<T> >(count, Buffer<T>(channels, frames))
{
    assert(count > 1 && channels && frames);
}

template <typename T>
RingBuffer<T>::~RingBuffer()
{
}

SOUND_INSTANTIATE(Sound::RingBuffer);
