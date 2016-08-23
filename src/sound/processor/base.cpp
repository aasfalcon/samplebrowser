#include "base.h"
#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

std::map<Command::Index, Base::Handler> Base::_handlers;
unsigned Base::_nextId = 0;

Base::Base()
    : _id(++_nextId)
{
}

Base::~Base()
{
}

void Base::call(Command::ID commandId)
{
    auto handler = this->_handlers.at(commandId);
    (this->*handler)();
}

void Base::commandInit()
{
    if (!empty()) {
        unsigned latency = get(Parameter::Processor::Latency);
        unsigned sampleRate = get(Parameter::Processor::SampleRate);

        for (auto it = this->begin(); it != this->end(); it++) {
            auto& child = *it;
            child->set(Parameter::Processor::Latency, latency);
            child->set(Parameter::Processor::SampleRate, sampleRate);
            child->set(Parameter::Processor::Parent, this);

            child->call(Command::Processor::Init);
        }
    }
}

bool Base::hasInternalBuffer()
{
    Base* parent = this->get(Parameter::Processor::Parent);
    return !parent
        || !this->empty()
            || bool(parent->get(Parameter::Processor::ChildrenParallel));
}

void Base::setParameterCount(unsigned count)
{
    _parameters.resize(count);
}

unsigned Base::id() const
{
    return _id;
}

void Base::setProperty(Property::ID id, const Any& value)
{
    if (!_properties[id].like(value)) {
        LOG(INFO, "Initialized with "
                << _properties[id].type_info().name()
                << " but attempted to replace with "
                << value.type_info().name());
        LOGIC_ERROR("Setting invalid property type");
    }

    _properties[id] = value;
}

RealtimeAny Base::get(Parameter::ID id) const
{
    unsigned index = id.as<unsigned>();
    return _parameters.at(index);
}

void Base::set(Parameter::ID id, RealtimeAny value)
{
    unsigned index = id.as<unsigned>();
    _parameters.assign(index, value);
}
