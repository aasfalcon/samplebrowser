#include "buffer.h"
#include "processor.h"

using namespace Sound;

template <typename T>
Processor<T>::Processor()
    : _in(nullptr)
    , _latency(0)
    , _out(nullptr)
    , _sampleRate(0)
{
}

template <typename T>
Processor<T>::~Processor()
{
}

template <typename T>
void Processor<T>::kickIn(Buffer<T>& out, Buffer<T>& in,
    unsigned latency, unsigned sampleRate)
{
    _out = &out;
    _in = &in;
    _latency = latency;
    _sampleRate = sampleRate;
    init();
}

SOUND_INSTANTIATE(Processor);
