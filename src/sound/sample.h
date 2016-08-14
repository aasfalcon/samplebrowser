#ifndef SAMPLE_H
#define SAMPLE_H

#include "sound.h"

template<typename T>
class Sample: public Object<T>
{
public:
    Sample();
    Sample(T value);
    Sample(const Sample &source);
    ~Sample();

    operator T() const;

private:
    T _value;
};

SOUND_INSTANTIATE_DECLARATION(Sample)

#endif // SAMPLE_H
