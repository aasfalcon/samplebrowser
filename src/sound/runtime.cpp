#include <cstring>

#include "runtime.h"
#include "shared/idriver.h"

using namespace Sound;

template <typename T>
Runtime<T>::Runtime(std::shared_ptr<Processor<T> > root,
    unsigned channels, unsigned frames,
    unsigned latency, unsigned sampleRate, Type sampleType)
    : _input(channels, frames)
    , _output(channels, frames)
    , _root(root)
    , _sampleType(sampleType)
{
    assert(channels && frames && root);
    _root->kickIn(_output, _input, latency, sampleRate);
}

template <typename T>
Runtime<T>::~Runtime()
{
}

template <typename T>
IDriver::Control Runtime<T>::process(const IDriver::ProcessParams& data)
{
    Runtime<T>* runtime = reinterpret_cast<Runtime<T>*>(data.object);

    if (data.input) {
        if (TypeInt24E == runtime->_sampleType) {
            runtime->_input.fromInt24(data.input);
        } else {
            std::memcpy(runtime->_input.data(), data.input,
                data.channelsInput * data.frames * sizeof(T));
        }
    }

    runtime->_root->process();

    if (data.output) {
        if (TypeInt24E == runtime->_sampleType) {
            runtime->_output.toInt24(data.output);
        } else {
            std::memcpy(data.output, runtime->_output.data(),
                data.channelsOutput * data.frames * sizeof(T));
        }
    }

    return IDriver::ControlContinue;
}

SOUND_INSTANTIATE(Runtime);
