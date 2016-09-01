#ifndef SOUND_OBJECT_H
#define SOUND_OBJECT_H

#include <climits>
#include <cstdint>
#include <type_traits>

#include "config.h"

namespace Sound {

#define SOUND_CONCAT_(a_name, a_bits) a_name ## a_bits
#define SOUND_CONCAT(a_name, a_bits) SOUND_CONCAT_(a_name, a_bits)

enum class Type : unsigned { // in order of increasing bit depth
    Int8,
    Int16,
    Int24E, // emulated with Float32
    Float32,
    Int32,
    Float64,
    Int64,
#ifdef SOUND_USE_LONG_DOUBLE
    Precise,
#endif
    Count,

    Int = SOUND_CONCAT(Int, SOUND_DEFAULT_INT_BITS),
    Float = SOUND_CONCAT(Float, SOUND_DEFAULT_FLOAT_BITS),
#ifndef SOUND_USE_LONG_DOUBLE
    Precise = Float64,
#endif
};

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef float Float32;
typedef std::int32_t Int32;
typedef double Float64;
typedef std::int64_t Int64;

#ifdef SOUND_USE_LONG_DOUBLE
typedef long double Precise;
#else
typedef Float64 Precise;
#endif

typedef SOUND_CONCAT(Int, SOUND_DEFAULT_INT_BITS) Int;
typedef SOUND_CONCAT(Float, SOUND_DEFAULT_FLOAT_BITS) Float;

typedef Float32 Int24E;

constexpr Int8 MAX_INT8 = INT8_MAX;
constexpr Int16 MAX_INT16 = INT16_MAX;
constexpr Float32 MAX_FLOAT32 = 1;
constexpr Int32 MAX_INT32 = INT32_MAX;
constexpr Float64 MAX_FLOAT64 = 1;
constexpr Int64 MAX_INT64 = INT64_MAX;
constexpr Precise MAX_PRECISE = 1;

constexpr Int MAX_INT = SOUND_CONCAT(MAX_INT, SOUND_DEFAULT_INT_BITS);
constexpr Float MAX_FLOAT = SOUND_CONCAT(MAX_FLOAT, SOUND_DEFAULT_FLOAT_BITS);

constexpr Int8 MIN_INT8 = INT8_MIN;
constexpr Int16 MIN_INT16 = INT16_MIN;
constexpr Float32 MIN_FLOAT32 = -1;
constexpr Int32 MIN_INT32 = INT32_MIN;
constexpr Float64 MIN_FLOAT64 = -1;
constexpr Int64 MIN_INT64 = INT64_MIN;
constexpr Precise MIN_PRECISE = -1;

constexpr Int MIN_INT = SOUND_CONCAT(MIN_INT, SOUND_DEFAULT_INT_BITS);
constexpr Float MIN_FLOAT = SOUND_CONCAT(MIN_FLOAT, SOUND_DEFAULT_FLOAT_BITS);

constexpr Int8 NIL_INT8 = 0;
constexpr Int16 NIL_INT16 = 0;
constexpr Float32 NIL_FLOAT32 = 0;
constexpr Int32 NIL_INT32 = 0;
constexpr Float64 NIL_FLOAT64 = 0;
constexpr Int64 NIL_INT64 = 0;
constexpr Precise NIL_PRECISE = 0;

constexpr Int NIL_INT = SOUND_CONCAT(NIL_INT, SOUND_DEFAULT_INT_BITS);
constexpr Float NIL_FLOAT = SOUND_CONCAT(NIL_FLOAT, SOUND_DEFAULT_FLOAT_BITS);

template <typename T>
class Object {
public:
    Object() {}
    ~Object() {}

    constexpr static bool isFloat()
    {
        return std::is_floating_point<T>::value;
    }

    constexpr static bool isInt() {
        return std::is_integral<T>::value;
    }

    constexpr static T min();
    constexpr static T max();
    constexpr static T nil() { return 0; }
    constexpr static Type type();
};
}

#include "object.tcc"

#endif // SOUND_OBJECT_H
