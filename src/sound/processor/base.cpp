#include "base.h"

using namespace Sound;
using namespace Sound::Processor;

std::map<unsigned, Base::Handler> Base::_handlers;
unsigned Base::_nextId = 0;

Base::Base()
    : _id(++_nextId)
{
}

Base::~Base()
{
}

#define SOUND_REGISTER_PROCESSOR_ONE(a_type, a_class) \
    _allocators[Type##a_type].addTag(#a_class,        \
    []() -> Base* { return new a_class<a_type>(); });

#define SOUND_REGISTER_PROCESSOR(a_class) \
    SOUND_ENUMERATE_TYPE(SOUND_REGISTER_PROCESSOR_ONE, a_class)

#include "register.h"

std::shared_ptr<Base> Base::allocate(
    const std::string& className, Type format)
{
    if (_allocators.empty()) {
        _allocators.resize(TYPE_COUNT);

        SOUND_REGISTER_PROCESSORS;
    }

    auto processor = std::shared_ptr<Base>(
        _allocators[format].create(className));

    return processor;
}

void Base::call(unsigned commandId)
{
    auto handler = this->_handlers.at(commandId);
    (this->*handler)();
}

void Base::commandInit()
{
    if (!empty()) {
        unsigned latency = get(Property::Processor::Latency_unsigned);
        unsigned sampleRate = get(Property::Processor::SampleRate_unsigned);

        for (auto it = this->begin(); it != this->end(); it++) {
            auto& child = *it;
            child->set(Property::Processor::Latency_unsigned, latency);
            child->set(Property::Processor::SampleRate_unsigned, sampleRate);
            child->set(Property::Processor::Parent_Sound_Processor_Base, this);

            child->call(Command::Processor::Init);
        }
    }
}

Any Base::get(unsigned propertyId) const
{
    return _properties.at(propertyId);
}

bool Base::hasInternalBuffer()
{
    Base *parent = this->get(Property::Processor::Parent_Sound_Processor_Base);
    return !parent
            || !this->empty()
            || bool(parent->get(Property::Processor::ChildrenParallel_bool));
}

unsigned Base::id() const
{
    return _id;
}

void Base::set(unsigned propertyId, const Any& value)
{
    _properties[propertyId] = value;
}
