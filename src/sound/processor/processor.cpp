#define PROCESSOR Processor

#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Processor<T>::Processor()
{
    COMMAND(Init);
    COMMAND(Process);

    PROPERTY(bool, ChildrenParallel, false);
    PROPERTY(bool, ChildrenAfter, false);
    PROPERTY(unsigned, Latency, 0);
    PROPERTY(Base*, Parent, nullptr);
    PROPERTY(Sound::Type, SampleFormat, this->type());
    PROPERTY(unsigned, SampleRate, 0);
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
void Processor<T>::commandProcess()
{
    bool isBypassed = this->get(Property::Processor::Bypass);

    if (isBypassed) {
        return;
    }

    bool isChildrenAfter = this->get(Property::Processor::ChildrenAfter);

    if (isChildrenAfter) {
        this->process();
    }

    if (!this->empty()) {
        auto beg = this->begin();
        auto end = this->end();
        bool isParallel = this->get(Property::Processor::ChildrenParallel);

        if (isParallel) {
            for (auto it = beg; it != end; ++it) {
                auto child = std::dynamic_pointer_cast<Processor<T> >(*it);
                child->_buffer.copy(this->_buffer.cbegin(), this->_buffer.cend());
                child->commandProcess();
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

                child->commandProcess();

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
}

template <typename T>
Processor<T>* Processor<T>::parent() const
{
    return get(Property::Processor::Parent);
}

INSTANTIATE;
