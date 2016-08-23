#include "base.h"

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

#define REGISTER_ONE(a_type, a_class)          \
    _allocators[Type##a_type].addTag(#a_class, \
        []() -> Base* { return new a_class<a_type>(); });

#define REGISTER(a_class) \
    SOUND_ENUMERATE_TYPE(REGISTER_ONE, a_class)

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

void Base::call(Command::ID commandId)
{
    auto handler = this->_handlers.at(commandId);
    (this->*handler)();
}

void Base::commandInit()
{
    if (!empty()) {
        unsigned latency = get(Property::Processor::Latency);
        unsigned sampleRate = get(Property::Processor::SampleRate);

        for (auto it = this->begin(); it != this->end(); it++) {
            auto& child = *it;
            child->set(Property::Processor::Latency, latency);
            child->set(Property::Processor::SampleRate, sampleRate);
            child->set(Property::Processor::Parent, this);

            child->call(Command::Processor::Init);
        }
    }
}

bool Base::hasInternalBuffer()
{
    Base* parent = this->get(Property::Processor::Parent);
    return !parent
        || !this->empty()
        || bool(parent->get(Property::Processor::ChildrenParallel_bool));
}

unsigned Base::id() const
{
    return _id;
}

void Base::set(Property::ID id, const Any& value)
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
