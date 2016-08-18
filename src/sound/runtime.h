#ifndef RUNTIME_H
#define RUNTIME_H

#include <memory>

#include "buffer.h"
#include "processor.h"

template <typename T>
class Runtime : public Sound::Object<T> {
public:
    Runtime(std::shared_ptr<Processor<T> > root,
        unsigned channels, unsigned frames,
        unsigned latency, unsigned sampleRate);
    ~Runtime();

    std::shared_ptr<Processor<T> > root();

private:
    Buffer<T> _inputBuffer;
    Buffer<T> _outputBuffer;
    std::shared_ptr<Processor<T> > _root;
};

SOUND_INSTANTIATION_DECLARE(Runtime);

#endif // RUNTIME_H
