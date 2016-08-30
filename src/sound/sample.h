#ifndef SOUND_SAMPLE_H
#define SOUND_SAMPLE_H

#include "object.h"
#include "dither.h"

namespace Sound {

template <typename T>
class Sample : public Object<T> {
public:
    inline Sample() {}

    inline Sample(T value)
        : _value(value)
    {
    }

    inline Sample(const Sample<T>& that)
        : _value(that._value)
    {
    }

    template <typename S>
    inline Sample(const Sample<S>& that)
        : _value(convert(that))
    {
    }

    inline const T& value() const { return _value; }
    inline operator const T&() const { return _value; }

private:
    T _value;

    template <typename S>
    static T convert(const Sample<S>& sample);

    template <typename S>
    inline constexpr static Precise ratio(bool isNegative, bool isDithering = false);
};

SOUND_INSTANTIATION_DECLARE(Sample);
}

#include "sample.tcc"

#endif // SOUND_SAMPLE_H
