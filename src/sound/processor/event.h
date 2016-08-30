#ifndef SOUND_PROCESSOR_EVENT_H
#define SOUND_PROCESSOR_EVENT_H

#include "shared/id.h"
#include "shared/value.h"

namespace Sound {
namespace Processor {

    struct Event {
        ID id;

        enum Kind : unsigned {
            ParameterToProcessor,
            ParameterFromProcessor,
            SignalFromProcessor,
        } kind;

        unsigned processor;
        Value value;

        Event();
        ~Event();

        Event& operator=(const Event& that);
    };
}
}

#endif // SOUND_PROCESSOR_EVENT_H
