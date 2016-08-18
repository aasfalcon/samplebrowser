#ifndef SAMPLE_H
#define SAMPLE_H

#include "sound.h"

template<typename T>
class Sample: public Sound::Object<T>
{
public:
    Sample();
    Sample(T value);
    Sample(const Sample &source);
    ~Sample();

    static T min();
    static T max();

    operator const T&() const;
    operator T &();

    template<typename S>
    Sample<T> &operator =(Sample<S> rht);

private:
    T _value;
};

#include "sample.tcc"

#endif // SAMPLE_H
