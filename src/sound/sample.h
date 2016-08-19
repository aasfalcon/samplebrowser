#ifndef SAMPLE_H
#define SAMPLE_H

#include <cmath>

#include "object.h"

namespace Sound {

template <typename T>
class Sample : public Object<T> {
public:
    Sample() {}
    Sample(T value)
        : _value(value)
    {
    }

    template <typename S>
    Sample(const Sample<S>& source)
        : _value(S(source))
    {
    }

    ~Sample() {}

    constexpr static bool isFloat();
    constexpr static bool isInt() { return !isFloat(); }
    constexpr static T min();
    constexpr static T max();

    operator const T&() const
    {
        return _value;
    }

    operator T&()
    {
        return _value;
    }

    template <typename S>
    inline Sample<T>& operator=(Sample<S> rht)
    {
        _value = T(S(rht));
        return *this;
    }

private:
    T _value;
};

template <>
constexpr bool Sample<Float32>::isFloat() { return true; }
template <>
constexpr bool Sample<Float64>::isFloat() { return true; }
template <>
constexpr bool Sample<Int8>::isFloat() { return false; }
template <>
constexpr bool Sample<Int16>::isFloat() { return false; }
template <>
constexpr bool Sample<Int32>::isFloat() { return false; }

template <>
constexpr Float32 Sample<Float32>::max() { return 1.0; }
template <>
constexpr Float64 Sample<Float64>::max() { return 1.0; }
template <>
constexpr Int8 Sample<Int8>::max() { return MaxInt8; }
template <>
constexpr Int16 Sample<Int16>::max() { return MaxInt16; }
template <>
constexpr Int32 Sample<Int32>::max() { return MaxInt32; }

template <>
constexpr Float32 Sample<Float32>::min() { return -1.0; }
template <>
constexpr Float64 Sample<Float64>::min() { return -1.0; }
template <>
constexpr Int8 Sample<Int8>::min() { return MinInt8; }
template <>
constexpr Int16 Sample<Int16>::min() { return MinInt16; }
template <>
constexpr Int32 Sample<Int32>::min() { return MinInt32; }

#define FLOAT_FROM_INT(__ltype, __rtype)                                    \
    template <>                                                             \
    template <>                                                             \
    inline Sample<__ltype>& Sample<__ltype>::operator=(Sample<__rtype> rht) \
    {                                                                       \
        _value = __ltype(double(__rtype(rht)) / double(Max##__rtype));      \
        return *this;                                                       \
    }

#define INT_FROM_FLOAT(__ltype, __rtype)                                    \
    template <>                                                             \
    template <>                                                             \
    inline Sample<__ltype>& Sample<__ltype>::operator=(Sample<__rtype> rht) \
    {                                                                       \
        _value = __ltype(std::lround(double(rht) * double(Max##__ltype)));  \
        return *this;                                                       \
    }

#define INT_FROM_INT(__ltype, __rtype, __shift)                             \
    template <>                                                             \
    template <>                                                             \
    inline Sample<__ltype>& Sample<__ltype>::operator=(Sample<__rtype> rht) \
    {                                                                       \
        _value = __ltype(int(__rtype(rht)) __shift);                        \
        return *this;                                                       \
    }

FLOAT_FROM_INT(Float32, Int8);
FLOAT_FROM_INT(Float32, Int16);
FLOAT_FROM_INT(Float32, Int32);
FLOAT_FROM_INT(Float64, Int8);
FLOAT_FROM_INT(Float64, Int16);
FLOAT_FROM_INT(Float64, Int32);

INT_FROM_FLOAT(Int8, Float32);
INT_FROM_FLOAT(Int16, Float32);
INT_FROM_FLOAT(Int32, Float32);
INT_FROM_FLOAT(Int8, Float64);
INT_FROM_FLOAT(Int16, Float64);
INT_FROM_FLOAT(Int32, Float64);

INT_FROM_INT(Int8, Int16, >> 8);
INT_FROM_INT(Int8, Int32, >> 24);
INT_FROM_INT(Int16, Int8, << 8);
INT_FROM_INT(Int16, Int32, >> 16);
INT_FROM_INT(Int32, Int8, << 24);
INT_FROM_INT(Int32, Int16, << 16);

#undef FLOAT_FROM_INT
#undef INT_FROM_FLOAT
#undef INT_FROM_INT
}

#endif // SAMPLE_H
