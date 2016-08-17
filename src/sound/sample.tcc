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
Sample<T>::operator T&() const
{
    return _value;
}
