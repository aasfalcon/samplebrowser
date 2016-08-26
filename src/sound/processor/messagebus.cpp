#include <mutex>

#include "config.h"
#include "messagebus.h"

using namespace Sound;
using namespace Sound::Processor;

MessageBus::MessageBus()
    : _commands(SOUND_PROCESSOR_COMMAND_BUS_SIZE)
    , _parameters(SOUND_PROCESSOR_PARAMETER_BUS_SIZE)
    , _signals(SOUND_PROCESSOR_SIGNAL_BUS_SIZE)

{
}

MessageBus::~MessageBus()
{
}

void MessageBus::addProcessor(Base* processor)
{
    _processors[processor->id()] = processor;
}

void MessageBus::addWatcher(MessageBus::Watcher watcher)
{
    addWatcherRecord(WatcherRecord::Global, watcher, 0, Signal::ID());
}

void MessageBus::addWatcher(MessageBus::Watcher watcher, unsigned processorId)
{
    addWatcherRecord(WatcherRecord::Processor, watcher, processorId, Signal::ID());
}

void MessageBus::addWatcher(MessageBus::Watcher watcher, unsigned processorId, Signal::ID signal)
{
    addWatcherRecord(WatcherRecord::ProcessorSignal, watcher, processorId, signal);
}

void MessageBus::addWatcher(MessageBus::Watcher watcher, Signal::ID signal)
{
    addWatcherRecord(WatcherRecord::Signal, watcher, 0, signal);
}

void MessageBus::clear()
{
    _commands.clear();
    _parameters.clear();
    _signals.clear();
}

void MessageBus::dispatch()
{
    if (_signals.lost()) {
        OVERFLOW_ERROR("Signals stack overflow");
    }

    std::mutex mutex;

    while (!_signals.isEmpty()) {
        mutex.lock();
        auto& message = _signals.pop();
        mutex.unlock();

        for (auto it = _watchers.begin(); it != _watchers.end(); ++it) {
            bool isHandler = it->scope == WatcherRecord::Global
                || (it->scope == WatcherRecord::Processor && it->processor == message.processor)
                || (it->scope == WatcherRecord::Signal && it->signal == message.signal)
                || (it->processor == message.processor && it->signal == message.signal);

            if (isHandler) {
                (*it->watcher)(message.processor, message.signal, message.value);
            }
        }
    }
}

void MessageBus::passParameter(unsigned processorId, Parameter::ID parameter,
    Value value)
{
    IncomingMessage message = {
        processorId,
        parameter,
        value,
    };

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

    if (_parameters.isFull()) {
        OVERFLOW_ERROR("Parameters stack overflow");
    }

    _parameters.push(message);
}

bool MessageBus::realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value)
{
    OutgoingMessage message = {
        processorId,
        signal,
        value,
    };

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);

    if (lock.owns_lock()) {
        _signals.push(message);
        return true;
    }

    return false;
}

void MessageBus::realtimeDispatch()
{
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);

    if (lock.owns_lock()) {
        _parameters.flush();

        while (!_parameters.isEmpty()) {
            const IncomingMessage& message = _parameters.pop();
            auto it = _processors.find(message.processor);

            if (it != _processors.end()) {
                it->second->set(message.parameter, message.value);
            }
        }
    }
}

void MessageBus::removeProcessor(unsigned processorId)
{
    _processors.erase(processorId);
}

void MessageBus::removeWatcher(MessageBus::Watcher watcher)
{
    for (auto it = _watchers.begin(); it != _watchers.end(); ++it) {
        if (it->watcher == watcher) {
            _watchers.erase(it);
        }
    }
}

void MessageBus::addWatcherRecord(MessageBus::WatcherRecord::Scope scope, Watcher watcher,
    unsigned processorId, Signal::ID signal)
{
    WatcherRecord record = {
        processorId,
        scope,
        signal,
        watcher,
    };

    _watchers.push_back(record);
}
