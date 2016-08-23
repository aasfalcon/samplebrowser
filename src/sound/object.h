#ifndef SOUND_OBJECT_H
#define SOUND_OBJECT_H

#include <climits>
#include <cstdint>

#include "soundmacros.h"

namespace Sound {

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef std::int32_t Int32;
typedef float Float32;
typedef double Float64;

typedef Float32 Int24E;

enum {
    MaxInt8 = SCHAR_MAX,
    MaxInt16 = SHRT_MAX,
    MaxInt32 = INT_MAX,
    MinInt8 = SCHAR_MIN,
    MinInt16 = SHRT_MIN,
    MinInt32 = INT_MIN,
};

enum Type {
    TypeFloat32,
    TypeFloat64,
    TypeInt8,
    TypeInt16,
    TypeInt24E, // emulated with Float32
    TypeInt32,
};

constexpr unsigned TYPE_COUNT = TypeInt32 + 1;

template <typename T>
class Object {
public:
    Object() {}
    ~Object() {}

    constexpr static Type type();
};

template <typename T>
struct always_false {
    enum { value = false };
};

template <typename T>
constexpr Type Object<T>::type()
{
    static_assert(always_false<T>::value,
        "Only Sound::IntXX/FloatXX types are supported as template parameter");
    return TypeFloat32;
}

#define SOUND_SPECIALIZE_OBJECT_TYPE(__type) \
    template <>                              \
    constexpr Type Object<__type>::type() { return Type##__type; }

SOUND_INSTANTIATE_METHOD(SOUND_SPECIALIZE_OBJECT_TYPE);
}

#endif // SOUND_OBJECT_H
