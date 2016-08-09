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

template<typename T>
Sample<T>::operator T() const
{
    return _value;
}

SOUND_INSTANTIATE(Sample)
