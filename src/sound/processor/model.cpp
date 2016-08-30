#include "model.h"
#include "messagebus.h"
#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

Model::Model(MessageBus& bus, unsigned processorId)
    : _bus(bus)
    , _isReady(false)
    , _processorId(processorId)
{
}

Model::~Model()
{
}

Value Model::get(Parameter::ID parameterId) const
{
    ERROR_IF(!isReady(), RUNTIME_ERROR, "Model is not ready");
    return _parameters[parameterId.toUInt()];
}

bool Model::isReady() const
{
    return _isReady;
}

void Model::perform(Command::ID commandId) const
{
    _bus.sendCommand(_processorId, commandId);
}

void Model::set(Parameter::ID parameterId, Value value)
{
    _parameters[parameterId.toUInt()] = value;
    _bus.sendParameter(_processorId, parameterId, value);
}

void Model::watch(const Event& event)
{
    ERROR_IF(event.processor != _processorId,
        RUNTIME_ERROR, "Wrong processor dispatched");

    if (Event::SignalFromProcessor == event.kind) {
        auto signalId = static_cast<Signal::Processor>(event.id.toUInt());

        if (Signal::Processor::ParameterCount == signalId && !_isReady) {
            unsigned count = event.value;
            _parameters.resize(count, Value::Null);
            _recieved.resize(count, false);
        } else if (Signal::Processor::ParametersSent == signalId) {
            for (auto it = _recieved.cbegin(); it != _recieved.cend(); ++it) {
                if (!*it) {
                    return;
                }
            }

            _isReady = true;
        }
    } else if (Event::ParameterFromProcessor == event.kind) {
        auto index = event.id.toUInt();
        _parameters[index] = event.value;
        _recieved[index] = true;
    }
}
