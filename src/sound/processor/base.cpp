#include "base.h"
#include "driver.h"
#include "messagebus.h"
#include "processor.h"

using namespace Sound;
using namespace Sound::Processor;

std::vector<Base::Handler> Base::_handlers;
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

void Base::perform(Command::ID id)
{
    auto handler = this->_handlers.at(id.toUInt());
    (this->*handler)();
}

void Base::commandInit()
{
    if (!empty()) {
        for (auto it = this->begin(); it != this->end(); it++) {
            auto& child = *it;
            child->_parent = this;
            child->set(Parameter::Processor::Runtime, &runtime());
            child->commandInit();
        }
    }

    _isInitialized = true;
}

void Base::send(Signal::ID id, RealtimeAny value)
{
    this->runtime().bus->realtimeEmitSignal(this->id(), id, value);
}

Base* Base::parent() const
{
    assert(_isInitialized);
    return _parent;
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
        this->send(Signal::Processor::Error, e.what());
    } catch (...) {
        this->send(Signal::Processor::Error, "Unknown exception");
    }
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

const RuntimeInfo& Base::runtime()
{
    RuntimeInfo* result = get(Parameter::Processor::Runtime);
    return *result;
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

bool Base::hasInternalBuffer()
{
    assert(_isInitialized);
    return !_parent
        || !this->empty()
        || bool(_parent->get(Parameter::Processor::ChildrenParallel));
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
    unsigned index = id.toUInt();

    if (!_properties[index].like(value)) {
        LOG(INFO, "Initialized with "
                << _properties[index].type_info().name()
                << " but attempted to replace with "
                << value.type_info().name());
        LOGIC_ERROR("Setting invalid property type");
    }

    _properties[index] = value;
}

RealtimeAny Base::get(Parameter::ID id) const
{
    unsigned index = id.toUInt();
    return _parameters.at(index);
}

void Base::set(Parameter::ID id, RealtimeAny value)
{
    unsigned index = id.toUInt();
    assert(index < _parameters.size());
    _parameters.at(index).assign(value);
}
