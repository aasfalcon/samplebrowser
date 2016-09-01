#include "processor.h"
#include "driver.h"
#include "mixer.h"
#include "shared/version.h"

#define PROCESSOR Processor
#include "shortcuts.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Processor<T>::Processor()
{
    INITIALIZE;

    COMMAND(Init);

    PROPERTY(std::string, Name, "Processor");
    PROPERTY(Version, Version, "1.0.0");

    PARAMETER(const RuntimeInfo*, Runtime, nullptr);

    PARAMETER(bool, Bypass, false);
    PARAMETER(bool, ChildrenParallel, false);
    PARAMETER(bool, ChildrenAfter, false);
}

template <typename T>
Processor<T>::~Processor()
{
}

template <typename T>
void Processor<T>::commandInit()
{
    if (hasInternalBuffer()) {
        auto parent = this->parent();

        if (parent) {
            auto& info = this->runtime();
            _output.reallocate(info.channelsOutput, info.frames);
        }

        if (this->size()) {
            _mixer = std::make_shared<Mixer<T> >();
        }
    }

    Base::commandInit();
}

template <typename T>
void Processor<T>::processChildrenParallel()
{
    const auto beg = this->begin();
    const auto end = this->end();

    for (auto it = beg; it != end; ++it) {
        auto child = static_cast<Processor<T>*>((*it).get());
        child->_output.copyFrom(_output.cbegin(), _output.cend());
        child->processStart();
    }

    _mixer->clear();

    for (auto it = beg; it != end; ++it) {
        auto child = static_cast<Processor<T>*>((*it).get());
        _mixer->add(child->_output.cbegin(), child->_output.cend());
    }

    _mixer->mixTo(_output.begin(), _output.end());
}

template <typename T>
void Processor<T>::processChildrenSerial()
{
    Buffer<T>* prev = &_output;

    for (auto it = this->begin(); it != this->end(); ++it) {
        auto child = static_cast<Processor<T>*>((*it).get());

        if (child->hasInternalBuffer()) {
            child->_output.copyFrom(prev->cbegin(), prev->cend());
            child->processStart();
            prev = &child->_output;
        } else {
            child->processStart();
        }
    }

    if (prev != &this->_output) {
        this->_output.copyFrom(prev->cbegin(), prev->cend());
    }
}

template <typename T>
Buffer<T>& Processor<T>::input()
{
    return parent()->input();
}

template <typename T>
Buffer<T>& Processor<T>::output()
{
    if (this->hasInternalBuffer()) {
        return _output;
    }

    return parent()->output();
}

template <typename T>
Processor<T>* Processor<T>::parent() const
{
    return static_cast<Processor<T>*>(Base::parent());
}

INSTANTIATE;
