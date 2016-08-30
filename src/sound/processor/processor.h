#ifndef SOUND_PROCESSOR_PROCESSOR_H
#define SOUND_PROCESSOR_PROCESSOR_H

#include <mutex>

#include "base.h"
#include "buffer.h"
#include "processormacros.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Processor, Base,
    ExposeParameters,
    Init);

SOUND_PROCESSOR_SIGNALS(Processor, Base,
    Error, // const char *
    ParameterCount, // unsigned
    ParametersSent // void
    );

SOUND_PROCESSOR_PARAMETERS(Processor, Base,
    Bypass, // bool
    ChildrenParallel, // bool
    ChildrenAfter, // bool
    Command, // Sound::Processor::Command::ID (last command on read)
    Runtime // Driver::RuntimeInfo *
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
        virtual Buffer<T>& input();
        Buffer<T>& output();

        void commandInit() override;

        void processChildrenParallel() override final;
        void processChildrenSerial() override final;

        Processor<T>* parent() const;

    private:
        Buffer<T> _output;
    };

    SOUND_INSTANTIATION_DECLARE(Processor);
}
}

#endif // SOUND_PROCESSOR_PROCESSOR_H
