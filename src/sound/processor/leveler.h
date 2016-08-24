#ifndef SOUND_PROCESSOR_LEVELER_H
#define SOUND_PROCESSOR_LEVELER_H

#include "processor.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Leveler, Processor, Void);

SOUND_PROCESSOR_PARAMETERS(Leveler, Processor,
    Balance, // double
    Level // double
    );

SOUND_PROCESSOR_PROPERTIES(Leveler, Processor, Void);
SOUND_PROCESSOR_SIGNALS(Leveler, Processor, Void);

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
