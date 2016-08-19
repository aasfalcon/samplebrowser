#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "object.h"

namespace Sound {

template <typename T>
class Buffer;

template <typename T>
class Processor : public Object<T> {
public:
    Processor();
    ~Processor();

    void kickIn(Buffer<T>& out, Buffer<T>& in,
        unsigned latency, unsigned sampleRate);
    virtual void process() = 0;

protected:
    Buffer<T>& in()
    {
        return *_in;
    }

    virtual void init() {}

    unsigned latency() const
    {
        return _latency;
    }

    Buffer<T>& out()
    {
        return *_out;
    }

    Type sampleFormat() const
    {
        return this->type();
    }

    unsigned sampleRate() const
    {
        return _sampleRate;
    }

private:
    Buffer<T>* _in;
    unsigned _latency;
    Buffer<T>* _out;
    unsigned _sampleRate;
};

SOUND_INSTANTIATION_DECLARE(Processor);
}

#endif // PROCESSOR_H
