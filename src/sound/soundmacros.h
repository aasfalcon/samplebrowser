#ifndef SOUND_SOUNDMACROS_H
#define SOUND_SOUNDMACROS_H

#include "id.h"

#define SOUND_ENUMERATE_TYPE(a_macro, a_param) \
    a_macro(Float32, a_param);                 \
    a_macro(Float64, a_param);                 \
    a_macro(Int8, a_param);                    \
    a_macro(Int16, a_param);                   \
    a_macro(Int32, a_param);

#define SOUND_INSTANTIATE_METHOD_ONE(a_type, a_macro) \
    a_macro(a_type)

#define SOUND_INSTANTIATE_METHOD(a_macro) \
    SOUND_ENUMERATE_TYPE(SOUND_INSTANTIATE_METHOD_ONE, a_macro)

#define SOUND_INSTANTIATE_ONE(a_type, a_class) \
    template class a_class<a_type>;

#define SOUND_INSTANTIATE(a_class) \
    SOUND_ENUMERATE_TYPE(SOUND_INSTANTIATE_ONE, a_class)

#define SOUND_INSTANTIATION_DECLARE_ONE(a_type, a_class) \
    extern template class a_class<a_type>;

#define SOUND_INSTANTIATION_DECLARE(a_class) \
    SOUND_ENUMERATE_TYPE(SOUND_INSTANTIATION_DECLARE_ONE, a_class)

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

#endif // SOUND_SOUNDMACROS_H
