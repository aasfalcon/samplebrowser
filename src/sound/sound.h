#ifndef SOUND_H
#define SOUND_H

#include <cstdint>
#include <climits>

namespace Sound {

typedef std::int8_t     Int8;
typedef std::int16_t    Int16;
typedef std::int32_t    Int32;
typedef float           Float32;
typedef double          Float64;

struct Int24 {
    Int24(std::int32_t val): value(val) {}
    Int24(double val): value(val) {}
    operator std::int32_t() const { return value; }
    operator std::int32_t &() { return value; }
    std::int32_t value;
};


enum MaxInt {
    MaxInt8     = SCHAR_MAX,
    MaxInt16    = SHRT_MAX,
    MaxInt24    = 8388607,
    MaxInt32    = INT_MAX,
};

enum MinInt {
    MinInt8     = SCHAR_MIN,
    MinInt16    = SHRT_MIN,
    MinInt24    = -8388608,
    MinInt32    = INT_MIN,
};

enum Quality {
    QualityRealtime,
    QualityFast = QualityRealtime,
    QualityMedium,
    QualityBest,

    QualityCount
};

enum Type {
    TypeFloat32,
    TypeFloat64,
    TypeInt8,
    TypeInt16,
    TypeInt24,
    TypeInt32,
};

const unsigned TypeCount = TypeInt32 + 1;

template<typename T>
class Object {
public:
    static Sound::Type type();
};

} // Sound

#include "sound.tcc"

#endif // SOUND_H
