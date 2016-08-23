#ifndef SOUND_PROCESSOR_LEVELER_H
#define SOUND_PROCESSOR_LEVELER_H

#include "processor.h"

namespace Sound {

SOUND_PROCESSOR_PARAMETERS(Leveler, Processor,
    Balance, // double
    Level // double
    );

namespace Processor {
    template <typename T>
    class Leveler : public Processor<T> {
    public:
        Leveler();
        ~Leveler() override;

    protected:
        virtual void process() override;
    };

    SOUND_PROCESSOR_FACTORY(Leveler);
}
}

#endif // SOUND_PROCESSOR_LEVELER_H
