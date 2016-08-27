#ifndef SOUND_PROCESSOR_MESSAGEBUS_H
#define SOUND_PROCESSOR_MESSAGEBUS_H

#include <list>
#include <map>

#include "base.h"
#include "shared/bus.h"
#include "shared/value.h"

namespace Sound {
namespace Processor {

    class MessageBus {
    public:
        typedef void (*Watcher)(unsigned processorId, Signal::ID signal, Value value);

        MessageBus();
        ~MessageBus();

        void addProcessor(Base* processor);
        void addWatcher(Watcher watcher);
        void addWatcher(Watcher watcher, unsigned processorId);
        void addWatcher(Watcher watcher, unsigned processorId, Signal::ID signal);
        void addWatcher(Watcher watcher, Signal::ID signal);
        void clear();
        void clearOverflows();
        void dispatch();
        void passParameter(unsigned processorId, Parameter::ID parameter, Value value);
        void requireCommand(unsigned processorId, Command::ID command);

        void realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value);
        void realtimeDispatch();

        void removeProcessor(unsigned processorId);
        void removeWatcher(Watcher watcher);

    private:
        struct CommandMessage {
            unsigned processor;
            Command::ID command;
        };

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

        Bus<CommandMessage> _commands;
        Bus<IncomingMessage> _parameters;
        std::map<unsigned, Base*> _processors;
        Bus<OutgoingMessage> _signals;
        std::list<WatcherRecord> _watchers;

        void addWatcherRecord(WatcherRecord::Scope scope, Watcher watcher,
            unsigned processorId, Signal::ID signal);
    };
}
}

#endif // SOUND_PROCESSOR_MESSAGEBUS_H
