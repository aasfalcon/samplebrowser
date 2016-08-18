#ifndef SAMPLE_H
#define SAMPLE_H

#include "sound.h"

template <typename T>
class Sample : public Sound::Object<T> {
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

    static T min();
    static T max();

    operator const T&() const
    {
        return _value;
    }

    operator T&()
    {
        return _value;
    }

    template <typename S>
    Sample<T>& operator=(Sample<S> rht)
    {
        _value = T(S(rht));
        return *this;
    }

private:
    T _value;
};

#endif // SAMPLE_H
