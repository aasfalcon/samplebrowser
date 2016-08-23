#ifndef SOUND_PROCESSOR_PROCESSOR_H
#define SOUND_PROCESSOR_PROCESSOR_H

#include <list>

#include "base.h"
#include "buffer.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Processor, Base,
    Init,
    Process);

SOUND_PROCESSOR_PROPERTIES(Processor, Base,
    Bypass, // bool
    ChildrenParallel, // bool
    ChildrenAfter, // bool
    Latency, // bool
    Parent, // Base *
    SampleFormat, // Sound::Type
    SampleRate); // Unsigned

namespace Processor {

    template <typename T>
    class Processor : public Base,
                      public Object<T> {
    public:
        Processor();
        ~Processor() override;

    protected:
        Buffer<T>& buffer();

        virtual void commandInit() override;
        virtual void commandProcess() final;

        Processor<T>* parent() const;

    private:
        Buffer<T> _buffer;
    };

    SOUND_INSTANTIATION_DECLARE(Processor);
}
}

#endif // SOUND_PROCESSOR_PROCESSOR_H
