#define PROCESSOR Processor

#include "processor.h"
#include "shared/version.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Processor<T>::Processor()
{
    USE_PARAMETERS;
    COMMAND(Init);

    PROPERTY(std::string, Name, "Processor");
    PROPERTY(Version, Version, "1.0.0");

    PARAMETER(bool, ChildrenParallel, false);
    PARAMETER(bool, ChildrenAfter, false);
    PARAMETER(unsigned, Latency, 0);
    PARAMETER(Base*, Parent, nullptr);
    PARAMETER(Sound::Type, SampleFormat, this->type());
    PARAMETER(unsigned, SampleRate, 0);
}

template <typename T>
Processor<T>::~Processor()
{
}

template <typename T>
Buffer<T>& Processor<T>::buffer()
{
    return hasInternalBuffer() ? _buffer : parent()->_buffer;
}

template <typename T>
void Processor<T>::commandInit()
{
    if (hasInternalBuffer()) {
        auto parent = this->parent();

        if (parent) {
            auto& parentBuffer = parent->buffer();
            _buffer.reallocate(parentBuffer.channels(), parentBuffer.frames());
        }
    }

    Base::commandInit();
}

template <typename T>
void Processor<T>::entryPoint()
{
    if (!_mutex.try_lock()) {
        return;
    }

    bool isBypassed = this->get(Parameter::Processor::Bypass);

    if (isBypassed) {
        return;
    }

    bool isChildrenAfter = this->get(Parameter::Processor::ChildrenAfter);

    if (isChildrenAfter) {
        this->process();
    }

    if (!this->empty()) {
        auto beg = this->begin();
        auto end = this->end();
        bool isParallel = this->get(Parameter::Processor::ChildrenParallel);

        if (isParallel) {
            for (auto it = beg; it != end; ++it) {
                auto child = std::dynamic_pointer_cast<Processor<T> >(*it);
                child->_buffer.copy(this->_buffer.cbegin(), this->_buffer.cend());
                child->entryPoint();
            }

            _buffer.silence();

            for (auto it = beg; it != end; ++it) {
                auto child = std::dynamic_pointer_cast<Processor<T> >(*it);
                _buffer.mix(child->_buffer.cbegin(), child->_buffer.cend(),
                    1.0 / double(this->size()));
            }
        } else {
            auto& prevBuffer = this->_buffer;

            for (auto it = beg; it != end; ++it) {
                auto child = std::dynamic_pointer_cast<Processor<T> >(*it);
                bool hasInternal = child->hasInternalBuffer();

                if (hasInternal) {
                    child->_buffer.copy(prevBuffer.cbegin(), prevBuffer.cend());
                }

                child->entryPoint();

                if (hasInternal) {
                    prevBuffer = child->_buffer;
                }
            }

            if (&prevBuffer != &this->_buffer) {
                this->_buffer.copy(prevBuffer.cbegin(), prevBuffer.cend());
            }
        }
    }

    if (!isChildrenAfter) {
        this->process();
    }

    _mutex.unlock();
}

template <typename T>
Processor<T>* Processor<T>::parent() const
{
    return get(Parameter::Processor::Parent);
}

INSTANTIATE;
