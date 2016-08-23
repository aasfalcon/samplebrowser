#ifndef SOUND_PROCESSOR_PROCESSORMACROS_H
#define SOUND_PROCESSOR_PROCESSORMACROS_H

#include "id.h"

#define ONE_SOUND_PROCESSOR_FACTORY(a_type, a_class)                \
    []() -> ::Sound::Processor::Base* {                             \
        return new ::Sound::Processor::a_class< ::Sound::a_type>(); \
    },

#define SOUND_PROCESSOR_FACTORY(a_class)                                   \
    SOUND_INSTANTIATION_DECLARE(a_class);                                  \
    namespace Factory {                                                    \
    struct a_class {                                                       \
        static ::Sound::Processor::Base* create(Type type)                 \
        {                                                                  \
            static ::Sound::Processor::Base* (*ctors[TYPE_COUNT])() = {    \
                SOUND_ENUMERATE_TYPE(ONE_SOUND_PROCESSOR_FACTORY, a_class) \
            };                                                             \
            return (*ctors[type])();                                       \
        }                                                                  \
    };                                                                     \
    }

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

#define SOUND_PROCESSOR_PARAMETERS(a_scope, a_base, ...) \
    SOUND_ENUM(Parameter, a_scope, a_base, __VA_ARGS__)

#define SOUND_PROCESSOR_PROPERTIES(a_scope, a_base, ...) \
    SOUND_ENUM(Property, a_scope, a_base, __VA_ARGS__)

#define SOUND_REGISTER_COMMAND(a_class, a_command) \
    Base::addCommand(                              \
        Command::a_class::a_command,               \
        &a_class<T>::command##a_command);

#define SOUND_REGISTER_PROPERTY(a_class, a_type, a_name, a_value) \
    this->setProperty(Property::a_class::a_name, static_cast<a_type>(a_value));

#define SOUND_REGISTER_PARAMETER(a_class, a_type, a_name, a_value) \
    this->set(Parameter::a_class::a_name, static_cast<a_type>(a_value));

#ifdef PROCESSOR
#define PROPERTY(a_type, a_name, a_value) \
    SOUND_REGISTER_PROPERTY(PROCESSOR, a_type, a_name, a_value)

#define PARAMETER(a_type, a_name, a_value) \
    SOUND_REGISTER_PARAMETER(PROCESSOR, a_type, a_name, a_value)

#define COMMAND(a_command) \
    SOUND_REGISTER_COMMAND(PROCESSOR, a_command)

#define INSTANTIATE \
    SOUND_INSTANTIATE(Sound::Processor::PROCESSOR)

#define USE_PARAMETERS \
    this->setParameterCount(unsigned(Parameter::PROCESSOR::_End));

#endif

#endif // SOUND_PROCESSOR_PROCESSORMACROS_H
