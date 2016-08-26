#include <mutex>

#include "config.h"
#include "messagebus.h"

using namespace Sound;
using namespace Sound::Processor;

MessageBus::MessageBus()
    : _parameters(SOUND_PROCESSOR_PARAMETER_BUS_SIZE)
    , _signals(SOUND_PROCESSOR_SIGNAL_BUS_SIZE)
    , _signalsHeld(SOUND_PROCESSOR_SIGNAL_BUS_HOLDED_SIZE)
{
}

MessageBus::~MessageBus()
{
}

void MessageBus::addProcessor(Base* processor)
{
    _processors[processor->id()] = processor;
}

void MessageBus::addWatcher(unsigned processorId, Signal::ID signal, MessageBus::Watcher watcher)
{
    addWatcherRecord(processorId, WatcherRecord::ProcessorSignal, signal, watcher);
}

void MessageBus::addWatcherGlobal(MessageBus::Watcher watcher)
{
    addWatcherRecord(0, WatcherRecord::Global, Signal::ID(), watcher);
}

void MessageBus::addWatcherProcessor(unsigned processorId, MessageBus::Watcher watcher)
{
    addWatcherRecord(processorId, WatcherRecord::Processor, Signal::ID(), watcher);
}

void MessageBus::addWatcherSignal(Signal::ID signal, MessageBus::Watcher watcher)
{
    addWatcherRecord(0, WatcherRecord::Global, signal, watcher);
}

void MessageBus::clear()
{
    _parameters.clear();
    _signals.clear();
    _signalsHeld.clear();
    _signalsLost = 0;
}

void MessageBus::dispatchSignals()
{
    if (_signalsLost) {
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

unsigned MessageBus::lostSignals() const
{
    return _signalsLost;
}

void MessageBus::lostClear()
{
    _signalsLost = 0;
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

void MessageBus::realtimeEmitSignal(unsigned processorId, Signal::ID signal, Value value)
{
    OutgoingMessage message = {
        processorId,
        signal,
        value,
    };

    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);

    if (lock.owns_lock() && !_signals.isFull()) {
        _signals.push(message);

        while (!_signals.isFull() && !_signalsHeld.isEmpty()) {
            _signals.push(_signalsHeld.pop());
        }
    } else {
        if (_signalsHeld.isFull()) { // overflow
            _signalsHeld.pop();
            ++_signalsLost;
        }

        _signalsHeld.push(message);
    }
}

void MessageBus::realtimeDispatchParameters()
{
    while (!_parameters.isEmpty()) {
        const IncomingMessage& message = _parameters.pop();
        auto it = _processors.find(message.processor);

        if (it != _processors.end()) {
            it->second->set(message.parameter, message.value);
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
            break;
        }
    }
}

void MessageBus::addWatcherRecord(unsigned processorId,
    MessageBus::WatcherRecord::Scope scope, Signal::ID signal,
    MessageBus::Watcher watcher)
{
    WatcherRecord record = {
        processorId,
        scope,
        signal,
        watcher,
    };

    _watchers.push_back(record);
}
