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
    ERROR_IF(!processor, RUNTIME_ERROR, "Processor is null");
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

void MessageBus::clearOverflows()
{
    _commands.clearLost();
    _parameters.clearLost();
    _signals.clearLost();
}

void MessageBus::dispatch()
{
    if (_commands.lost()) {
        OVERFLOW_ERROR("Command bus overflow");
    }

    if (_parameters.lost()) {
        OVERFLOW_ERROR("Parameter bus overflow");
    }

    if (_signals.lost()) {
        OVERFLOW_ERROR("Signal bus overflow");
    }

    std::mutex mutex;

    try {
        std::lock_guard<std::mutex> lock(mutex);
        _commands.flush();
        _parameters.flush();
        _signals.flush();
    } catch (...) {
        throw;
    }

    while (!_signals.isEmpty()) {
        OutgoingMessage message;

        try {
            std::lock_guard<std::mutex> lock(mutex);
            message = _signals.pop();
        } catch (...) {
            throw;
        }

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
    _parameters.push(message);
}

void MessageBus::requireCommand(unsigned processorId, Command::ID command)
{
    CommandMessage message = {
        processorId,
        command,
    };

    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    _commands.push(message);
}

void MessageBus::realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value)
{
    OutgoingMessage message = {
        processorId,
        signal,
        value,
    };

    _signals.push(message);
}

void MessageBus::realtimeDispatch()
{
    while (!_parameters.isEmpty()) {
        const IncomingMessage& message = _parameters.pop();
        auto it = _processors.find(message.processor);

        if (it != _processors.end()) {
            it->second->set(message.parameter, message.value);
        } else {
            LOG(ERROR, "Parameter reciever processor ID "
                    << message.processor
                    << " is not registered");
            RUNTIME_ERROR("No such processor");
        }
    }

    while (!_commands.isEmpty()) {
        const CommandMessage& message = _commands.pop();

        auto it = _processors.find(message.processor);

        if (it != _processors.end()) {
            it->second->perform(message.command);
        } else {
            LOG(ERROR, "Command reciever processor ID "
                    << message.processor
                    << " is not registered, command is: "
                    << message.command.toUInt());
            RUNTIME_ERROR("No such processor");
        }
    }
}

void MessageBus::removeProcessor(unsigned processorId)
{
    auto it = _processors.find(processorId);

    if (it == _processors.end()) {
        LOG(ERROR, "Attempt to remove non-attached processor "
                << processorId << " from bus");
        RUNTIME_ERROR("No such processor");
    }

    _processors.erase(processorId);
}

void MessageBus::removeWatcher(MessageBus::Watcher watcher)
{
    bool isFound = false;

    for (auto it = _watchers.begin(); it != _watchers.end(); ++it) {
        if (it->watcher == watcher) {
            auto del = it;
            ++it;
            _watchers.erase(del);
            isFound = true;
        }
    }

    if (!isFound) {
        RUNTIME_ERROR("No such watcher");
    }
}

void MessageBus::addWatcherRecord(MessageBus::WatcherRecord::Scope scope, Watcher watcher,
    unsigned processorId, Signal::ID signal)
{
    ERROR_IF(!watcher, RUNTIME_ERROR, "Watcher is null");

    WatcherRecord record = {
        processorId,
        scope,
        signal,
        watcher,
    };

    _watchers.push_back(record);
}
