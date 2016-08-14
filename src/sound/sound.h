#ifndef SOUND_H
#define SOUND_H

#include <cstdint>
#include <climits>

namespace Sound {

typedef std::int8_t     Int8;
typedef std::int16_t    Int16;
typedef std::int32_t    Int24; // emulated
typedef std::int32_t    Int32;
typedef float           Float32;
typedef double          Float64;

enum MaxInt {
    MaxInt8     = SCHAR_MAX,
    MaxInt16    = SHRT_MAX,
    MaxInt24    = 8388607,
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
    __macro(Int32)

#define _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, __type) \
    __macro(__type, Float32) \
    __macro(__type, Float64) \
    __macro(__type, Int8) \
    __macro(__type, Int16) \
    __macro(__type, Int32)

#define SOUND_SPECIALIZE_COMPLEX(__macro) \
    _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, Float32) \
    _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, Float64) \
    _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, Int8) \
    _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, Int16) \
    _SOUND_SPECIALIZE_COMPLEX_SINGLE(__macro, Int32)

#define _SOUND_INSTANTIATE(__class, __extern) \
    __extern template class __class<Sound::Float32>; \
    __extern template class __class<Sound::Float64>; \
    __extern template class __class<Sound::Int8>; \
    __extern template class __class<Sound::Int16>; \
    __extern template class __class<Sound::Int32>;

#define SOUND_INSTANTIATE(__class) \
    _SOUND_INSTANTIATE(__class,)
#define SOUND_INSTANTIATE_DECLARATION(__class) \
    _SOUND_INSTANTIATE(__class, extern)

#define __OBJECT_TYPE(__type) \
    template<> Sound::Type Object<Sound::__type>::type();

SOUND_SPECIALIZE(__OBJECT_TYPE)
SOUND_INSTANTIATE_DECLARATION(Object)

#undef __OBJECT_TYPE

#endif // SOUND_H
