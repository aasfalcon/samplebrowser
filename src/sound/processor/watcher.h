#ifndef SOUND_PROCESSOR_WATCHER_H
#define SOUND_PROCESSOR_WATCHER_H

#include "base.h"
#include "event.h"

namespace Sound {
namespace Processor {

    class Watcher {
    public:
        virtual void watch(const Event &event) = 0;
    };
}
}

#endif // SOUND_PROCESSOR_WATCHER_H
