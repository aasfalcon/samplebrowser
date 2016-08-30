#ifndef SOUND_OBJECT_H
#define SOUND_OBJECT_H

#include <climits>
#include <cstdint>

namespace Sound {

enum class Type : unsigned { // in order of increasing bit depth
    Int8,
    Int16,
    Int24E, // emulated with Float32
    Float32,
    Int32,
    Int = Int32, // best int
    Float64,
    Float = Float64, // best float
    Precise, // better or equal to Float
    Count = Precise
};

typedef std::int8_t Int8;
typedef std::int16_t Int16;
typedef std::int32_t Int32;
typedef float Float32;
typedef double Float64;

typedef long double Precise;
#define DECLARE_PRECISE_METHODS

typedef Float32 Int24E;
typedef Int32 Int;
typedef Float64 Float;

constexpr Float32 MAX_FLOAT32 = 1.0F;
constexpr Float64 MAX_FLOAT64 = 1.0;
constexpr Int8 MAX_INT8 = INT8_MAX;
constexpr Int16 MAX_INT16 = INT16_MAX;
constexpr Int32 MAX_INT32 = INT32_MAX;

constexpr Int MAX_INT = MAX_INT32;
constexpr Float MAX_FLOAT = MAX_FLOAT64;
constexpr Precise MAX_PRECISE = 1.0L;

constexpr Float32 MIN_FLOAT32 = -1.0F;
constexpr Float64 MIN_FLOAT64 = -1.0;
constexpr Int8 MIN_INT8 = INT8_MIN;
constexpr Int16 MIN_INT16 = INT16_MIN;
constexpr Int32 MIN_INT32 = INT32_MIN;

constexpr Int MIN_INT = MIN_INT32;
constexpr Float MIN_FLOAT = MIN_FLOAT64;
constexpr Precise MIN_PRECISE = -1.0L;

constexpr Float32 NIL_FLOAT32 = 0.0F;
constexpr Float64 NIL_FLOAT64 = 0.0;
constexpr Int8 NIL_INT8 = 0;
constexpr Int16 NIL_INT16 = 0;
constexpr Int32 NIL_INT32 = 0;

constexpr Int NIL_INT = NIL_INT32;
constexpr Float NIL_FLOAT = NIL_FLOAT64;
constexpr Precise NIL_PRECISE = 0.0L;

template <typename T>
class Object {
public:
    inline Object() {}
    inline ~Object() {}

    inline constexpr static bool isFloat()
    {
        return type() == Type::Float
            || type() == Type::Float32
            || type() == Type::Float64
            || type() == Type::Precise;
    }

    inline constexpr static bool isInt() { return !isFloat(); }
    inline constexpr static T min();
    inline constexpr static T max();
    inline constexpr static T nil() { return 0; }
    inline constexpr static Type type();
};
}

#include "object.tcc"

#endif // SOUND_OBJECT_H
