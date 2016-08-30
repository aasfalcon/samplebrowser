#include "base.h"
#include "driver.h"
#include "messagebus.h"
#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

unsigned Base::_nextId = 0;

Base::Base()
    : _id(++_nextId)
    , _isInitialized(false)
    , _parent(nullptr)
{
}

Base::~Base()
{
}

void Base::addCommand(Command::ID id, Base::Handler handler)
{
    unsigned index = id.toUInt();
    assert(index < _commands.size());
    _commands[index] = static_cast<Handler>(handler);
}

void Base::commandInit()
{
    _runtime = get(Parameter::Processor::Runtime);

    if (!empty()) {
        for (auto it = this->begin(); it != this->end(); it++) {
            auto& child = *it;
            child->_parent = this;
            child->set(Parameter::Processor::Runtime, _runtime);
            child->commandInit();
        }
    }

    _isInitialized = true;
}

void Base::commandExposeParameters()
{
    emitSignal(Signal::Processor::ParameterCount, _parameters.size());

    unsigned index = 0;

    for (auto it = _parameters.cbegin(); it != _parameters.cend(); ++it, ++index) {
        emitParamteter(static_cast<Parameter::Processor>(index), *it);
    }

    emitSignal(Signal::Processor::ParametersSent);
}

void Base::emitSignal(Signal::ID signalId, Value value)
{
    this->runtime().bus->realtimeEmitSignal(this->id(), signalId, value);
}

void Base::emitParamteter(Parameter::ID parameterId, Value value)
{
    this->runtime().bus->realtimeEmitParameter(this->id(), parameterId, value);
}

Value Base::get(Parameter::ID id) const
{
    unsigned index = id.toUInt();
    return _parameters.at(index);
}

bool Base::hasInternalBuffer()
{
    return !_parent
        || !this->empty()
            || bool(_parent->get(Parameter::Processor::ChildrenParallel));
}

unsigned Base::id() const
{
    return _id;
}

void Base::initVectors(Command::ID nCommands, Parameter::ID nParameters, Property::ID nProperties)
{
    _commands.resize(nCommands.toUInt());
    _parameters.resize(nParameters.toUInt());
    _properties.resize(nProperties.toUInt());
}

Base* Base::parent() const
{
    return _parent;
}

void Base::perform(Command::ID id)
{
    auto handler = this->_commands.at(id.toUInt());
    (this->*handler)();
}

void Base::processEntryPoint()
{
    assert(_isInitialized);

    try {
        bool isBypassed = this->get(Parameter::Processor::Bypass);

        if (!isBypassed) {
            this->processPre();
            this->process();
            this->processPost();
        }
    } catch (std::exception e) {
        this->emitSignal(Signal::Processor::Error, e.what());
    } catch (...) {
        this->emitSignal(Signal::Processor::Error, "Unknown exception");
    }
}

Any Base::property(Property::ID id) const
{
    return _properties.at(id.toUInt());
}

void Base::processPost()
{
    if (this->size()) {
        bool childrenAfter = this->get(Parameter::Processor::ChildrenAfter);

        if (childrenAfter) {
            bool childrenParallel = this->get(Parameter::Processor::ChildrenParallel);

            if (childrenParallel) {
                processChildrenParallel();
            } else {
                processChildrenSerial();
            }
        }
    }
}

void Base::processPre()
{
    if (this->size()) {
        bool childrenAfter = this->get(Parameter::Processor::ChildrenAfter);

        if (!childrenAfter) {
            bool childrenParallel = this->get(Parameter::Processor::ChildrenParallel);

            if (childrenParallel) {
                processChildrenParallel();
            } else {
                processChildrenSerial();
            }
        }
    }
}

const RuntimeInfo& Base::runtime()
{
    const RuntimeInfo* result = get(Parameter::Processor::Runtime);
    return *result;
}

void Base::set(Parameter::ID id, Value value)
{
    unsigned index = id.toUInt();
    assert(index < _parameters.size());
    _parameters.at(index) = value;
    emitParamteter(id, value);
}

void Base::setProperty(Property::ID id, const Any& value)
{
    unsigned index = id.toUInt();

    if (!_properties[index].like(value)) {
        LOG(DEBUG, "Initialized with "
                << _properties[index].type_info().name()
                << " but attempted to replace with "
                << value.type_info().name());
        LOGIC_ERROR("Setting invalid property type");
    }

    assert(index < _properties.size());
    _properties[index] = value;
}

