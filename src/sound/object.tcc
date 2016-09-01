#ifndef SOUND_OBJECT_TCC
#define SOUND_OBJECT_TCC

#ifndef SOUND_OBJECT_H
#include "object.h"
#endif

#include "soundmacros.h"

namespace Sound {

template <>
inline constexpr Int8 Object<Int8>::max() { return MAX_INT8; }
template <>
inline constexpr Int16 Object<Int16>::max() { return MAX_INT16; }
template <>
inline constexpr Float32 Object<Float32>::max() { return MAX_FLOAT32; }
template <>
inline constexpr Int32 Object<Int32>::max() { return MAX_INT32; }
template <>
inline constexpr Float64 Object<Float64>::max() { return MAX_FLOAT64; }
template <>
inline constexpr Int64 Object<Int64>::max() { return MAX_INT64; }

#ifdef SOUND_USE_LONG_DOUBLE
template <>
inline constexpr Precise Object<Precise>::max()
{
    return MAX_PRECISE;
}
#endif

template <>
inline constexpr Int8 Object<Int8>::min()
{
    return MIN_INT8;
}
template <>
inline constexpr Int16 Object<Int16>::min() { return MIN_INT16; }
template <>
inline constexpr Float32 Object<Float32>::min() { return MIN_FLOAT32; }
template <>
inline constexpr Int32 Object<Int32>::min() { return MIN_INT32; }
template <>
inline constexpr Float64 Object<Float64>::min() { return MIN_FLOAT64; }
template <>
inline constexpr Int64 Object<Int64>::min() { return MIN_INT64; }

#ifdef SOUND_USE_LONG_DOUBLE
template <>
inline constexpr Precise Object<Precise>::min()
{
    return MIN_PRECISE;
}
#endif

template <typename T>
struct AlwaysFalse {
    enum { Value = false };
};

template <typename T>
constexpr Type Object<T>::type()
{
    static_assert(AlwaysFalse<T>::Value,
        "Only Sound::IntXX/FloatXX types are supported as template parameter");
    return static_cast<Type>(0);
}

#define SOUND_SPECIALIZE_OBJECT_TYPE(__type) \
    template <>                              \
    inline constexpr Type Object<__type>::type() { return Type::__type; }

SOUND_INSTANTIATE_METHOD(SOUND_SPECIALIZE_OBJECT_TYPE);
}

#endif // SOUND_OBJECT_TCC
