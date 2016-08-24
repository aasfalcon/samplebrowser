#include "realtimeany.h"

namespace Sound {

template <>
void RealtimeAny::assign<bool>(bool value)
{
    _value.vBool = value;
}

template <>
void RealtimeAny::assign<double>(double value)
{
    _value.vDouble = value;
}

template <>
void RealtimeAny::assign<int>(int value)
{
    _value.vInt = value;
}

template <>
void RealtimeAny::assign<void*>(void* value)
{
    _value.vPointer = value;
}

template <>
void RealtimeAny::assign<unsigned>(unsigned value)
{
    _value.vUnsigned = value;
}

template <>
bool RealtimeAny::as<bool>() const
{
    assert(&typeid(bool) == _type);
    return _value.vBool;
}

template <>
double RealtimeAny::as<double>() const
{
    assert(&typeid(double) == _type);
    return _value.vDouble;
}

template <>
int RealtimeAny::as<int>() const
{
    assert(&typeid(int) == _type);
    return _value.vInt;
}

template <>
void* RealtimeAny::as<void*>() const
{
    assert(&typeid(void*) == _type);
    return _value.vPointer;
}

template <>
unsigned RealtimeAny::as<unsigned>() const
{
    assert(&typeid(unsigned) == _type);
    return _value.vUnsigned;
}
}
