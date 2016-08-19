#include "runtime.h"

using namespace Sound;

template <typename T>
Runtime<T>::Runtime(std::shared_ptr<Processor<T> > root,
    unsigned channels, unsigned frames,
    unsigned latency, unsigned sampleRate)
    : _input(channels, frames)
    , _output(channels, frames)
    , _root(root)
{
    _root->kickIn(_output, _input, latency, sampleRate);
}

template <typename T>
Runtime<T>::~Runtime()
{
}

SOUND_INSTANTIATE(Runtime);
