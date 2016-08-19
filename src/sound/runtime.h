#ifndef RUNTIME_H
#define RUNTIME_H

#include <memory>

#include "buffer.h"
#include "processor.h"

namespace Sound {

template <typename T>
class Runtime : public Object<T> {
public:
    Runtime(std::shared_ptr<Processor<T> > root,
        unsigned channels, unsigned frames,
        unsigned latency, unsigned sampleRate);
    ~Runtime();

    std::shared_ptr<Processor<T> > root()
    {
        return _root;
    }

private:
    Buffer<T> _input;
    Buffer<T> _output;
    std::shared_ptr<Processor<T> > _root;
};

SOUND_INSTANTIATION_DECLARE(Runtime);
}

#endif // RUNTIME_H
