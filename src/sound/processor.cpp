#include "processor.h"

template <typename T>
Processor<T>::Processor()
{
}

template <typename T>
Processor<T>::~Processor()
{
}

template <typename T>
void Processor<T>::init()
{
}

template <typename T>
Buffer<T>* Processor<T>::in()
{
    return _in;
}

template <typename T>
void Processor<T>::kickIn(Buffer<T>* out, Buffer<T>* in,
    unsigned latency, unsigned sampleRate)
{
    _out = out;
    _in = in;
    _latency = latency;
    _sampleRate = sampleRate;
    init();
}

template <typename T>
unsigned Processor<T>::latency() const
{
    return _latency;
}

template <typename T>
Buffer<T>* Processor<T>::out()
{
    return _out;
}

template <typename T>
Sound::Type Processor<T>::sampleFormat() const
{
    return this->type();
}

template <typename T>
unsigned Processor<T>::sampleRate() const
{
    return _sampleRate;
}

SOUND_INSTANTIATE(Processor);
