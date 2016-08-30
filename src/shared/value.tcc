#ifndef SHARED_VALUE_TCC
#define SHARED_VALUE_TCC

#ifndef SHARED_VALUE_H
#include "shared/value.h"
#endif

template <>
inline bool Value::as<bool>() const
{
    ERROR_IF(type() != typeid(bool), BAD_CAST, "Value is not bool");
    return _data.intVal;
}

template <>
inline float Value::as<float>() const
{
    ERROR_IF(type() != typeid(float), BAD_CAST, "Value is not float");
    return _data.floatVal;
}

template <>
inline double Value::as<double>() const
{
    return static_cast<double>(as<float>());
}

template <>
inline int Value::as<int>() const
{
    ERROR_IF(type() != typeid(int), BAD_CAST, "Value is not int");
    return _data.intVal;
}

template <>
inline char Value::as<char>() const
{
    return static_cast<char>(as<int>());
}

template <>
inline signed char Value::as<signed char>() const
{
    return static_cast<signed char>(as<int>());
}

template <>
inline unsigned char Value::as<unsigned char>() const
{
    return static_cast<unsigned char>(as<int>());
}

template <>
inline short Value::as<short>() const
{
    return static_cast<short>(as<int>());
}

template <>
inline unsigned short Value::as<unsigned short>() const
{
    return static_cast<unsigned short>(as<int>());
}

template <>
inline unsigned int Value::as<unsigned int>() const
{
    return static_cast<unsigned int>(as<int>());
}

template <>
inline long long Value::as<long long>() const
{
    return static_cast<long long>(as<int>());
}

template <>
inline unsigned long long Value::as<unsigned long long>() const
{
    return static_cast<unsigned long long>(as<int>());
}

#endif // SHARED_VALUE_TCC
