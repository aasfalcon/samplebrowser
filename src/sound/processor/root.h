#ifndef SOUND_PROCESSOR_ROOT_H
#define SOUND_PROCESSOR_ROOT_H

#include "processor.h"

namespace Sound {

SOUND_PROCESSOR_PARAMETERS(Root, Processor,
    ChannelsInput, // unsigned
    ChannelsOutput, // unsigned
    Frames, // unsigned
    Status, // unsigned (IDriver::Status flags)
    );

namespace Processor {

    template <typename T>
    class Root : public Processor<T> {
    protected:
        void commandInit() override;
        void process() override;
    };

    SOUND_PROCESSOR_FACTORY(Silence);
}
}

#endif // SOUND_PROCESSOR_ROOT_H
