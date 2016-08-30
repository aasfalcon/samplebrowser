#include <algorithm>
#include <mutex>

#include "config.h"
#include "messagebus.h"
#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

MessageBus::MessageBus()
    : _fromRealtime(SOUND_PROCESSOR_BUS_SIZE, SOUND_PROCESSOR_BUS_HOLD_FACTOR)
    , _toRealtime(SOUND_PROCESSOR_BUS_SIZE, SOUND_PROCESSOR_BUS_HOLD_FACTOR)
{
}

MessageBus::~MessageBus()
{
}

void MessageBus::addProcessor(Base& processor)
{
    unsigned id = processor.id();
    ERROR_IF(_processors.find(id) != _processors.end(),
        RUNTIME_ERROR, "Processor already present");
    _processors[id] = &processor;
}

void MessageBus::addWatcher(Watcher& watcher, unsigned processorId)
{
    WatcherRecord record = { processorId, &watcher };

    auto it = std::find_if(_watchers.begin(), _watchers.end(),
        [record](const WatcherRecord& check) -> bool {
            return check.processor == record.processor
                && check.watcher == record.watcher;
        });

    ERROR_IF(it != _watchers.end(), RUNTIME_ERROR, "Watcher already present");
    _watchers.push_back(record);
}

void MessageBus::clearBus()
{
    _fromRealtime.clear();
    _toRealtime.clear();
}

void MessageBus::dispatch()
{
    std::mutex mutex;

    try {
        std::lock_guard<std::mutex> lock(mutex);

        if (_fromRealtime.lost()) {
            stressOffload();
        } else {
            _fromRealtime.load();
            _toRealtime.load();
        }
    } catch (...) {
        throw;
    }

    while (!_fromRealtime.isEmpty()) {
        Event event;

        try {
            std::lock_guard<std::mutex> lock(mutex);
            event = _fromRealtime.pop();
        } catch (...) {
            throw;
        }

        for (auto it = _watchers.begin(); it != _watchers.end(); ++it) {
            if (!it->processor || (it->processor == event.processor)) {
                it->watcher->watch(event);
            }
        }
    }
}

bool MessageBus::hasProcessor(unsigned processorId) const
{
    return _processors.find(processorId) != _processors.end();
}

bool MessageBus::hasWatcher(Watcher& watcher) const
{
    return std::find_if(_watchers.begin(), _watchers.end(),
               [&watcher](const WatcherRecord& check) {
                   return check.watcher == &watcher;
               })
        != _watchers.end();
}

void MessageBus::send(Event::Kind kind, unsigned processorId, ID id, Value value)
{
    Event event;

    event.id = id;
    event.kind = kind;
    event.processor = processorId;
    event.value = value;

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    try {
        _toRealtime.push(event);
    } catch (std::overflow_error) {
        stressOffload();
        _toRealtime.push(event);
    }
}

void MessageBus::sendCommand(unsigned processorId, Command::ID command)
{
    send(Event::ParameterToProcessor, processorId,
         Parameter::Processor::Command, command.toUInt());
}

void MessageBus::sendParameter(unsigned processorId, Parameter::ID parameter, Value value)
{
    send(Event::ParameterToProcessor, processorId, parameter, value);
}

void MessageBus::realtimeEmit(Event::Kind kind, unsigned processorId, ID id, Value value)
{
    Event event;
    event.id = id;
    event.kind = kind;
    event.processor = processorId;
    event.value = value;
    _fromRealtime.push(event);
}

void MessageBus::realtimeEmitParameter(unsigned processorId, Parameter::ID parameter, Value value)
{
    realtimeEmit(Event::ParameterFromProcessor, processorId, parameter, value);
}

void MessageBus::realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value)
{
    realtimeEmit(Event::SignalFromProcessor, processorId, signal, value);
}

void MessageBus::realtimeDispatch()
{
    while (!_toRealtime.isEmpty()) {
        const Event& event = _toRealtime.pop();
        auto it = _processors.find(event.processor);

        if (it != _processors.end()) {
            auto processor = it->second;

            if (Event::ParameterToProcessor == event.kind) {
                processor->set(event.id, event.value);

                if (Parameter::Processor::Command == event.value) {
                    auto command = static_cast<Command::Processor>(event.value.as<unsigned>());
                    processor->perform(command);
                }
            }
        }
    }
}

void MessageBus::removeProcessor(unsigned processorId)
{
    auto it = _processors.find(processorId);
    ERROR_IF(it == _processors.end(), RUNTIME_ERROR, "No such processor");
    _processors.erase(it);
}

void MessageBus::removeWatcher(Watcher* watcher)
{
    bool isFound = false;
    auto it = _watchers.begin();

    do {
        it = std::find_if(it, _watchers.end(),
            [watcher](const WatcherRecord& check) -> bool {
                return check.watcher == watcher;
            });

        if (it != _watchers.end()) {
            isFound = true;
            it = _watchers.erase(it);
        }
    } while (it != _watchers.end());

    ERROR_IF(!isFound, RUNTIME_ERROR, "No such watcher");
}

void MessageBus::stressOffload()
{
    _fromRealtime.clearLost();
    _toRealtime.clearLost();

    while (_fromRealtime.held() || _toRealtime.held()) {
        realtimeDispatch();
        dispatch();
    }

    realtimeDispatch();
}
