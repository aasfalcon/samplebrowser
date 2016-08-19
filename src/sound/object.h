#ifndef SOUND_H
#define SOUND_H

#include <climits>
#include <cstdint>

namespace Sound {

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef std::int32_t Int32;
typedef float Float32;
typedef double Float64;

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
    TypeInt32,
};

const unsigned TYPE_COUNT = TypeInt32 + 1;

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

template <>
constexpr Type Object<Float32>::type() { return TypeFloat32; }
template <>
constexpr Type Object<Float64>::type() { return TypeFloat64; }
template <>
constexpr Type Object<Int8>::type() { return TypeInt8; }
template <>
constexpr Type Object<Int16>::type() { return TypeInt16; }
template <>
constexpr Type Object<Int32>::type() { return TypeInt32; }

#define SOUND_INSTANTIATE_METHOD(__macro) \
    __macro(Float32);                     \
    __macro(Float64);                     \
    __macro(Int8);                        \
    __macro(Int16);                       \
    __macro(Int32);

#define SOUND_INSTANTIATE(__class)       \
    namespace Sound {                    \
        template class __class<Float32>; \
        template class __class<Float64>; \
        template class __class<Int8>;    \
        template class __class<Int16>;   \
        template class __class<Int32>;   \
    }

#define SOUND_INSTANTIATION_DECLARE(__class) \
    extern template class __class<Float32>;  \
    extern template class __class<Float64>;  \
    extern template class __class<Int8>;     \
    extern template class __class<Int16>;    \
    extern template class __class<Int32>;
}

#endif // SOUND_H
