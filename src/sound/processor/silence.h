#ifndef SOUND_PROCESSOR_SILENCE_H
#define SOUND_PROCESSOR_SILENCE_H

#include "processor.h"

namespace Sound {
namespace Processor {

    template <typename T>
    class Silence : public Processor<T> {
    protected:
        void process() override;
    };

    SOUND_INSTANTIATION_DECLARE(Silence);
}
}

#endif // SOUND_PROCESSOR_SILENCE_H
