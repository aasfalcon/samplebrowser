#ifndef SOUND_SOUNDMACROS_H
#define SOUND_SOUNDMACROS_H

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

#define SOUND_NAMESPACE_ENUM(a_ns, a_derived, a_base, ...) \
    namespace a_ns {                                       \
    namespace a_derived {                                  \
        enum {                                             \
            _Begin = Sound::a_ns::a_base::_End,            \
            __VA_ARGS__,                                   \
            _End,                                          \
        };                                                 \
    }                                                      \
    }

#define SOUND_COMMANDS(a_derived, a_base, ...) \
    SOUND_NAMESPACE_ENUM(Command, a_derived, a_base, __VA_ARGS__)

#define SOUND_PROPERTIES(a_derived, a_base, ...) \
    SOUND_NAMESPACE_ENUM(Property, a_derived, a_base, __VA_ARGS__)

#define SOUND_REGISTER_COMMAND(a_className, a_commandName) \
    Base::registerCommand(                                 \
        Command::a_className::a_commandName,               \
        &a_className<T>::command##a_commandName);

#endif // SOUND_SOUNDMACROS_H
