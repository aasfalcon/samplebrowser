#ifndef SOUND_PROCESSOR_PROCESSOR_H
#define SOUND_PROCESSOR_PROCESSOR_H

#include <list>
#include <mutex>

#include "base.h"
#include "buffer.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Processor, Base,
    Init);

SOUND_PROCESSOR_PARAMETERS(Processor, Base,
    Bypass, // bool
    ChildrenParallel, // bool
    ChildrenAfter, // bool
    Latency, // bool
    Parent, // Base *
    SampleFormat, // Sound::Type
    SampleRate // unsigned
    );

SOUND_PROCESSOR_PROPERTIES(Processor, Base,
    Name, // std::string
    Version // Version
    );

namespace Processor {

    template <typename T>
    class Processor : public Base,
                      public Object<T> {
    public:
        Processor();
        ~Processor() override;

    protected:
        std::mutex _mutex;

        Buffer<T>& buffer();

        virtual void commandInit() override;
        virtual void entryPoint() final;

        Processor<T>* parent() const;

    private:
        Buffer<T> _buffer;
    };

    SOUND_INSTANTIATION_DECLARE(Processor);
}
}

#endif // SOUND_PROCESSOR_PROCESSOR_H
