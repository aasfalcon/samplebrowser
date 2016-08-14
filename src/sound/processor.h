#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <map>
#include <memory>
#include "buffer.h"

template<typename T>
class Processor: public Object<T>
{
public:
    Processor();
    ~Processor();

    void kickIn(std::shared_ptr<Buffer<T> > out,
                std::shared_ptr<Buffer<T> > in,
                unsigned latency, unsigned sampleRate);
    virtual void process() = 0;

protected:
    std::shared_ptr<Buffer<T> > &in();
    virtual void init();
    unsigned latency() const;
    std::shared_ptr<Buffer<T> > &out();
    Sound::Type sampleFormat() const;
    unsigned sampleRate() const;

private:
    std::shared_ptr<Buffer<T>> _in;
    unsigned _latency;
    std::shared_ptr<Buffer<T>> _out;
    unsigned _sampleRate;
};

SOUND_INSTANTIATE_DECLARATION(Processor)

#endif // PROCESSOR_H

