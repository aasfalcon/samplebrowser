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
    Bypass_bool,
    ChildrenParallel_bool,
    ChildrenAfter_bool,
    Latency_unsigned,
    Parent_Sound_Processor_Base,
    SampleFormat_Sound_Type,
    SampleRate_unsigned);

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

        Processor<T>* parent();

    private:
        Buffer<T> _buffer;
    };

    SOUND_INSTANTIATION_DECLARE(Processor);
}
}

#endif // SOUND_PROCESSOR_PROCESSOR_H
