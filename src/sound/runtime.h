#ifndef SOUND_RUNTIME_H
#define SOUND_RUNTIME_H

#include <memory>

#include "buffer.h"
#include "processor/processor.h"
#include "shared/idriver.h"

namespace Sound {

template <typename T>
class Runtime : public Object<T> {
public:
    Runtime(std::shared_ptr<Processor<T> > root,
        unsigned channels, unsigned frames,
        unsigned latency, unsigned sampleRate, Type sampleType);
    ~Runtime();

    std::shared_ptr<Processor<T> > root()
    {
        return _root;
    }

    static IDriver::Control process(const IDriver::ProcessParams &data);

private:
    Buffer<T> _input;
    Buffer<T> _output;
    std::shared_ptr<Processor<T> > _root;
    Type _sampleType;
};

SOUND_INSTANTIATION_DECLARE(Runtime);
}

#endif // SOUND_RUNTIME_H
