#ifndef SOUND_SAMPLE_H
#define SOUND_SAMPLE_H

#include "dither.h"
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

    Sample(const Sample<T>& that)
        : _value(that._value)
    {
    }

    template <typename S>
    Sample(const Sample<S>& that)
        : _value(convert(that))
    {
    }

    void silence()
    {
        _value = this->nil();
    }

    T& value() { return _value; }
    const T& value() const { return _value; }

private:
    T _value;

    template <typename S>
    static T convert(const Sample<S>& sample);

    template <typename S>
    constexpr static Precise ratio(bool isNegative, bool isDithering = false);
};

SOUND_INSTANTIATION_DECLARE(Sample);
}

#include "sample.tcc"

#endif // SOUND_SAMPLE_H
