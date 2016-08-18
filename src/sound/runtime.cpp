#include "runtime.h"

template <typename T>
Runtime<T>::Runtime(std::shared_ptr<Processor<T> > root,
    unsigned channels, unsigned frames,
    unsigned latency, unsigned sampleRate)
    : _inputBuffer(channels, frames)
    , _outputBuffer(channels, frames)
    , _root(root)
{
    _root->kickIn(&_outputBuffer, &_inputBuffer, latency, sampleRate);
}

template <typename T>
Runtime<T>::~Runtime()
{
}

template <typename T>
std::shared_ptr<Processor<T> > Runtime<T>::root()
{
    return _root;
}

SOUND_INSTANTIATE(Runtime);
