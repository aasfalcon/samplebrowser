#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>
#include <memory>
#include "buffer.h"

template<typename T>
class Processor: public Sound::Object<T>
{
public:
    Processor();
    ~Processor();

    void kickIn(Buffer<T> *out,
                Buffer<T> *in,
                unsigned latency, unsigned sampleRate);
    virtual void process() = 0;

protected:
    Buffer<T> *in();
    virtual void init();
    unsigned latency() const;
    Buffer<T> *out();
    Sound::Type sampleFormat() const;
    unsigned sampleRate() const;

private:
    Buffer<T> *_in;
    unsigned _latency;
    Buffer<T> *_out;
    unsigned _sampleRate;
};

#include "processor.tcc"

#endif // PROCESSOR_H
