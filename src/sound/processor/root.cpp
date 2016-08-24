#define PROCESSOR Root

#include "root.h"
#include "buffer.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Root<T>::Root()
{
    this->setProperty(Property::Processor::Name, "Root");
}

template <typename T>
Root<T>::~Root()
{
}

template <typename T>
void Root<T>::commandInit()
{
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    auto &info = this->runtime();
    if (!info.channelsInput && !info.channelsOutput) {
        LOGIC_ERROR("Both input and output channels count is zero");
    }

    if (!info.frames) {
        LOGIC_ERROR("Zero buffer size");
    }

    if (!info.sampleRate) {
        LOGIC_ERROR("Base sample rate is not set");
    }

    if (info.channelsInput) {
        auto &in = this->input();
        in.reallocate(info.channelsInput, info.frames);
    }

    if (info.channelsOutput) {
        auto &out = this->output();
        out.reallocate(info.channelsOutput, info.frames);
    }

    this->runtime().bus->clear();
    Processor<T>::commandInit();
}

template <typename T>
Buffer<T>& Root<T>::input()
{
    return _inputBuffer;
}

template <typename T>
void Root<T>::processPre()
{
    auto &info = this->runtime();
    info.bus->realtimeDispatchParameters();

    if (info.rawInput) {
        auto &in = this->input();

        if (TypeInt24E != info.sampleFormat) {
            auto frame = ConstFrame<T>(info.channelsInput, info.rawInput);
            in->copy(frame, frame + info.frames);
        } else {
            in->fromInt24(info.rawInput);
        }
    }

    Processor<T>::processPre();
}

template <typename T>
void Root<T>::processPost()
{
    Processor<T>::processPost();

    auto &info = this->runtime();

    if (info.rawOutput) {
        auto &out = this->output();

        if (TypeInt24E != info.sampleFormat) {
            auto frame = Frame<T>(info.channelsInput, info.rawOutput);
            out->copyTo(frame, frame + info->frames);
        } else {
            out->toInt24(info.rawOutput);
        }
    }
}
