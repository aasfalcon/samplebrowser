#ifndef SHARED_VALUE_TCC
#define SHARED_VALUE_TCC

#ifndef SHARED_VALUE_H
#include "shared/value.h"
#endif

template <>
inline bool Value::as<bool>() const
{
    ERROR_IF(type() != typeid(bool), BAD_CAST, "Value is not bool");
    return _data.bv;
}

template <>
inline float Value::as<float>() const
{
    ERROR_IF(type() != typeid(float), BAD_CAST, "Value is not float");
    return _data.fv;
}

template <>
inline int Value::as<int>() const
{
    ERROR_IF(type() != typeid(int), BAD_CAST, "Value is not int");
    return _data.iv;
}

template <>
inline unsigned Value::as<unsigned>() const
{
    ERROR_IF(type() != typeid(unsigned), BAD_CAST, "Value is not unsigned int");
    return _data.uv;
}

#endif // SHARED_VALUE_TCC
