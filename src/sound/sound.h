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

enum MaxInt {
    MaxInt8 = SCHAR_MAX,
    MaxInt16 = SHRT_MAX,
    MaxInt32 = INT_MAX,
};

enum MinInt {
    MinInt8 = SCHAR_MIN,
    MinInt16 = SHRT_MIN,
    MinInt32 = INT_MIN,
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
    TypeInt32,
};

const unsigned TypeCount = TypeInt32 + 1;

template <typename T>
class Object {
public:
    static Sound::Type type();
};

} // Sound

#define SOUND_INSTANTIATE_METHOD(__macro) \
    __macro(Sound::Float32);              \
    __macro(Sound::Float64);              \
    __macro(Sound::Int8);                 \
    __macro(Sound::Int16);                \
    __macro(Sound::Int32);

#define _TEMPLATE_METHOD(__macro, __ttype) \
    __macro(__ttype, Sound::Float32);      \
    __macro(__ttype, Sound::Float64);      \
    __macro(__ttype, Sound::Int8);         \
    __macro(__ttype, Sound::Int16);        \
    __macro(__ttype, Sound::Int32);

#define SOUND_INSTANTIATE_TEMPLATE_METHOD(__macro) \
    _TEMPLATE_METHOD(__macro, Sound::Float32);     \
    _TEMPLATE_METHOD(__macro, Sound::Float64);     \
    _TEMPLATE_METHOD(__macro, Sound::Int8);        \
    _TEMPLATE_METHOD(__macro, Sound::Int16);       \
    _TEMPLATE_METHOD(__macro, Sound::Int32);

#define SOUND_INSTANTIATE(__class)          \
    template class __class<Sound::Float32>; \
    template class __class<Sound::Float64>; \
    template class __class<Sound::Int8>;    \
    template class __class<Sound::Int16>;   \
    template class __class<Sound::Int32>;

#define SOUND_INSTANTIATION_DECLARE(__class)       \
    extern template class __class<Sound::Float32>; \
    extern template class __class<Sound::Float64>; \
    extern template class __class<Sound::Int8>;    \
    extern template class __class<Sound::Int16>;   \
    extern template class __class<Sound::Int32>;

#endif // SOUND_H
