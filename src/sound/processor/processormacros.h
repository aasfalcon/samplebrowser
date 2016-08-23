#ifndef SOUND_PROCESSOR_PROCESSORMACROS_H
#define SOUND_PROCESSOR_PROCESSORMACROS_H

#include "id.h"

#define SOUND_ENUM_NAMESPACE(a_ns) \
    namespace a_ns {               \
    enum class Base : unsigned {   \
        _Begin,                    \
        _End = _Begin,             \
    };                             \
    typedef Sound::ID ID;          \
    typedef Sound::Index Index;    \
    }

#define SOUND_ENUM(a_ns, a_scope, a_base, ...) \
    namespace a_ns {                           \
    enum class a_scope : unsigned {            \
        _Begin = unsigned(a_base::_End),       \
        __VA_ARGS__,                           \
        _End,                                  \
    };                                         \
    }

#define SOUND_PROCESSOR_COMMANDS(a_scope, a_base, ...) \
    SOUND_ENUM(Command, a_scope, a_base, __VA_ARGS__)

#define SOUND_PROCESSOR_PROPERTIES(a_scope, a_base, ...) \
    SOUND_ENUM(Property, a_scope, a_base, __VA_ARGS__)

#define SOUND_REGISTER_COMMAND(a_class, a_command) \
    Base::addCommand(                              \
        Command::a_class::a_command,               \
        &a_class<T>::command##a_command);

#define SOUND_REGISTER_PROPERTY(a_class, a_type, a_name, a_value) \
    this->set(Property::a_class::a_name, static_cast<a_type>(a_value));

#ifdef PROCESSOR
namespace Sound {
namespace Processor {
}
}
using namespace Sound;
using namespace Sound::Processor;

#define PROPERTY(a_type, a_name, a_value) \
    SOUND_REGISTER_PROPERTY(PROCESSOR, a_type, a_name, a_value)

#define COMMAND(a_command) \
    SOUND_REGISTER_COMMAND(PROCESSOR, a_command)

#define INSTANTIATE \
    SOUND_INSTANTIATE(Sound::Processor::PROCESSOR)
#endif

#endif // SOUND_PROCESSOR_PROCESSORMACROS_H
