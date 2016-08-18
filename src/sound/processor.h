#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "buffer.h"
#include <map>
#include <memory>

template <typename T>
class Processor : public Sound::Object<T> {
public:
    Processor();
    ~Processor();

    void kickIn(Buffer<T>* out,
        Buffer<T>* in,
        unsigned latency, unsigned sampleRate);
    virtual void process() = 0;

protected:
    Buffer<T>* in();
    virtual void init();
    unsigned latency() const;
    Buffer<T>* out();
    Sound::Type sampleFormat() const;
    unsigned sampleRate() const;

private:
    Buffer<T>* _in;
    unsigned _latency;
    Buffer<T>* _out;
    unsigned _sampleRate;
};

SOUND_INSTANTIATION_DECLARE(Processor);

#endif // PROCESSOR_H
