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

#endif // SOUND_SOUNDMACROS_H
