#ifndef SOUND_PROCESSOR_MESSAGEBUS_H
#define SOUND_PROCESSOR_MESSAGEBUS_H

#include <list>
#include <map>

#include "base.h"
#include "event.h"
#include "shared/bus.h"
#include "watcher.h"

namespace Sound {
namespace Processor {

    class MessageBus {
    public:
        MessageBus();
        ~MessageBus();

        void addProcessor(Base& processor);
        void addWatcher(Watcher& watcher, unsigned processorId = 0);
        void clearBus();
        void dispatch();
        bool hasProcessor(unsigned processorId) const;
        bool hasWatcher(Watcher& watcher) const;
        void sendCommand(unsigned processorId, Command::ID command);
        void sendParameter(unsigned processorId, Parameter::ID parameter, Value value);

        void realtimeEmitParameter(unsigned processorId, Parameter::ID parameter, Value value);
        void realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value);
        void realtimeDispatch();

        void removeProcessor(unsigned processorId);
        void removeWatcher(Watcher* watcher);

    private:
        Bus<Event> _fromRealtime;
        Bus<Event> _toRealtime;
        std::map<unsigned, Base*> _processors;

        struct WatcherRecord {
            unsigned processor;
            Watcher* watcher;
        };

        std::list<WatcherRecord> _watchers;

        void realtimeEmit(Event::Kind kind, unsigned processorId, ID id, Value value);
        void send(Event::Kind kind, unsigned processorId, ID id, Value value);
        void stressOffload();
    };
}
}

#endif // SOUND_PROCESSOR_MESSAGEBUS_H
