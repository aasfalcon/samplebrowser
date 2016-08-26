#ifndef SOUND_PROCESSOR_MESSAGEBUS_H
#define SOUND_PROCESSOR_MESSAGEBUS_H

#include <list>
#include <map>

#include "base.h"
#include "shared/value.h"
#include "shared/ring.h"

namespace Sound {
namespace Processor {

    class MessageBus {
    public:
        typedef void (*Watcher)(unsigned processorId, Signal::ID signal, Value value);

        MessageBus();
        ~MessageBus();

        void addProcessor(Base* processor);
        void addWatcher(unsigned processorId, Signal::ID signal, Watcher watcher);
        void addWatcherGlobal(Watcher watcher);
        void addWatcherProcessor(unsigned processorId, Watcher watcher);
        void addWatcherSignal(Signal::ID signal, Watcher watcher);
        void clear();
        void dispatchSignals();
        unsigned lostSignals() const;
        void lostClear();
        void passParameter(unsigned processorId, Parameter::ID parameter, Value value);

        void realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value);
        void realtimeDispatchParameters();

        void removeProcessor(unsigned processorId);
        void removeWatcher(Watcher watcher);

    private:
        struct IncomingMessage {
            unsigned processor;
            Parameter::ID parameter;
            Value value;
        };

        struct OutgoingMessage {
            unsigned processor;
            Signal::ID signal;
            Value value;
        };

        struct WatcherRecord {
            enum Scope {
                Global,
                Processor,
                ProcessorSignal,
                Signal,
            };

            unsigned processor;
            Scope scope;
            Signal::ID signal;
            Watcher watcher;
        };

        Ring<IncomingMessage> _parameters;
        std::map<unsigned, Base*> _processors;
        Ring<OutgoingMessage> _signals;
        Ring<OutgoingMessage> _signalsHeld;
        unsigned _signalsLost;
        std::list<WatcherRecord> _watchers;

        void addWatcherRecord(unsigned processorId, WatcherRecord::Scope scope,
            Signal::ID signal, Watcher watcher);
    };
}
}

#endif // SOUND_PROCESSOR_MESSAGEBUS_H
