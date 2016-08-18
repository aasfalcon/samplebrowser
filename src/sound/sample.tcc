#include <stdexcept>
#include "sample.h"

template<typename T>
Sample<T>::Sample()
{

}

template<typename T>
Sample<T>::Sample(T value)
    : _value(value)
{

}

template<typename T>
Sample<T>::Sample(const Sample &source)
    : _value(source._value)
{

}

template<typename T>
Sample<T>::~Sample()
{

}

template<> Sound::Float32 Sample<Sound::Float32>::min() { return 1.0; }
template<> Sound::Float64 Sample<Sound::Float64>::min() { return 1.0; }
template<> Sound::Int8 Sample<Sound::Int8>::min() { return Sound::MinInt8; }
template<> Sound::Int16 Sample<Sound::Int16>::min() { return Sound::MinInt16; }
template<> Sound::Int24 Sample<Sound::Int24>::min() { return Sound::MinInt24; }
template<> Sound::Int32 Sample<Sound::Int32>::min() { return Sound::MinInt32; }

template<> Sound::Float32 Sample<Sound::Float32>::max() { return 1.0; }
template<> Sound::Float64 Sample<Sound::Float64>::max() { return 1.0; }
template<> Sound::Int8 Sample<Sound::Int8>::max() { return Sound::MaxInt8; }
template<> Sound::Int16 Sample<Sound::Int16>::max() { return Sound::MaxInt16; }
template<> Sound::Int24 Sample<Sound::Int24>::max() { return Sound::MaxInt24; }
template<> Sound::Int32 Sample<Sound::Int32>::max() { return Sound::MaxInt32; }

template<typename T>
Sample<T>::operator T&()
{
    return _value;
}

template<typename T>
template<typename S>
Sample<T> &Sample<T>::operator =(Sample<S> rht)
{
    _value = rht._value;
    return *this;
}

#define FLOAT_FROM_INT(__ltype, __rtype) \
template<> template<> Sample<Sound::__ltype> & \
    Sample<Sound::__ltype>::operator=(Sample<Sound::__rtype> rht) { \
        _value = Sound::__ltype(Sound::__rtype(rht)) / Sound::Max##__rtype; \
        return *this; \
    }

FLOAT_FROM_INT(Float32, Int8 );
FLOAT_FROM_INT(Float32, Int16);
FLOAT_FROM_INT(Float32, Int24);
FLOAT_FROM_INT(Float32, Int32);

FLOAT_FROM_INT(Float64, Int8 );
FLOAT_FROM_INT(Float64, Int16);
FLOAT_FROM_INT(Float64, Int24);
FLOAT_FROM_INT(Float64, Int32);

#undef FLOAT_FROM_INT
#define INT_FROM_FLOAT(__ltype, __rtype) \
template<> template<> Sample<Sound::__ltype> & \
    Sample<Sound::__ltype>::operator=(Sample<Sound::__rtype> rht) { \
        _value = Sound::__ltype(Sound::__rtype(rht) * Sound::Max##__ltype); \
        return *this; \
    }

INT_FROM_FLOAT(Int8,  Float32);
INT_FROM_FLOAT(Int16, Float32);
INT_FROM_FLOAT(Int24, Float32);
INT_FROM_FLOAT(Int32, Float32);

INT_FROM_FLOAT(Int8,  Float64);
INT_FROM_FLOAT(Int16, Float64);
INT_FROM_FLOAT(Int24, Float64);
INT_FROM_FLOAT(Int32, Float64);

#undef INT_FROM_FLOAT
#define INT_FROM_INT(__ltype, __rtype, __shift) \
template<> template<> Sample<Sound::__ltype> & \
    Sample<Sound::__ltype>::operator=(Sample<Sound::__rtype> rht) { \
        _value = Sound::__ltype(int(Sound::__rtype(rht)) __shift); \
        return *this; \
    }

INT_FROM_INT(Int8,  Int16, >> 8 );
INT_FROM_INT(Int8,  Int24, >> 16);
INT_FROM_INT(Int8,  Int32, >> 24);

INT_FROM_INT(Int16, Int8,  << 8 );
INT_FROM_INT(Int16, Int24, >> 8 );
INT_FROM_INT(Int16, Int32, >> 16);

INT_FROM_INT(Int24, Int8,  << 16);
INT_FROM_INT(Int24, Int16, << 8 );
INT_FROM_INT(Int24, Int32, >> 8 );

INT_FROM_INT(Int32, Int8,  << 24);
INT_FROM_INT(Int32, Int16, << 16);
INT_FROM_INT(Int32, Int24, << 8 );

#undef INT_FROM_INT
