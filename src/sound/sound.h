#ifndef SOUND_H
#define SOUND_H

#include <cstdint>
#include <climits>

namespace Sound {

#include "int24.h"

typedef std::int8_t     Int8;
typedef std::int16_t    Int16;
typedef Int24           Int24;
typedef std::int32_t    Int32;
typedef float           Float32;
typedef double          Float64;

enum MaxInt {
    MaxInt8     = SCHAR_MAX,
    MaxInt16    = SHRT_MAX,
    MaxInt24    = INT24_MAX,
    MaxInt32    = INT_MAX,
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

    TypeCount
};

} // Sound

template<typename T>
class Object {
public:
    virtual ~Object();
    static Sound::Type type();
};

#define SOUND_SPECIALIZE(__macro) \
    __macro(Float32) \
    __macro(Float64) \
    __macro(Int8) \
    __macro(Int16) \
    __macro(Int24) \
    __macro(Int32)

#define SOUND_INSTANTIATE(__class) \
    template class __class<Sound::Float32>; \
    template class __class<Sound::Float64>; \
    template class __class<Sound::Int8>; \
    template class __class<Sound::Int16>; \
    template class __class<Sound::Int24>; \
    template class __class<Sound::Int32>;

#endif // SOUND_H
