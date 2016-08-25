#include "realtimeany.h"

namespace Sound {

template <>
inline void RealtimeAny::assign<bool>(bool value)
{
    _value.vBool = value;
}

template <>
inline void RealtimeAny::assign<float>(float value)
{
    _value.vFloat = value;
}

template <>
inline void RealtimeAny::assign<int>(int value)
{
    _value.vInt = value;
}

template <>
inline void RealtimeAny::assign<void*>(void* value)
{
    _value.vPtr = value;
}

template <>
inline void RealtimeAny::assign<unsigned>(unsigned value)
{
    _value.vUInt = value;
}

template <>
inline bool RealtimeAny::as<bool>() const
{
    assert(&typeid(bool) == _type);
    return _value.vBool;
}

template <>
inline float RealtimeAny::as<float>() const
{
    assert(&typeid(float) == _type);
    return _value.vFloat;
}

template <>
inline int RealtimeAny::as<int>() const
{
    assert(&typeid(int) == _type);
    return _value.vInt;
}

template <>
inline void* RealtimeAny::as<void*>() const
{
    assert(&typeid(void*) == _type);
    return _value.vPtr;
}

template <>
inline unsigned RealtimeAny::as<unsigned>() const
{
    assert(&typeid(unsigned) == _type);
    return _value.vUInt;
}
}
