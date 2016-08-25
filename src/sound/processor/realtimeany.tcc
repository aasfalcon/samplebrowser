#include "realtimeany.h"

namespace Sound {

template <>
inline void RealtimeAny::assign<bool>(bool value)
{
    _value.vBool = value;
}

template <>
inline void RealtimeAny::assign<double>(double value)
{
    _value.vDouble = value;
}

template <>
inline void RealtimeAny::assign<int>(int value)
{
    _value.vInt = value;
}

template <>
inline void RealtimeAny::assign<void*>(void* value)
{
    _value.vPointer = value;
}

template <>
inline void RealtimeAny::assign<unsigned>(unsigned value)
{
    _value.vUnsigned = value;
}

template <>
inline bool RealtimeAny::as<bool>() const
{
    assert(&typeid(bool) == _type);
    return _value.vBool;
}

template <>
inline double RealtimeAny::as<double>() const
{
    assert(&typeid(double) == _type);
    return _value.vDouble;
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
    return _value.vPointer;
}

template <>
inline unsigned RealtimeAny::as<unsigned>() const
{
    assert(&typeid(unsigned) == _type);
    return _value.vUnsigned;
}
}
