#ifndef SOUND_SAMPLE_TCC
#define SOUND_SAMPLE_TCC

#ifndef SOUND_SAMPLE_H
#include "sample.h"
#endif

namespace Sound {

template <typename T>
template <typename S>
inline constexpr Precise Sample<T>::ratio(bool isNegative, bool isDithering)
{
    return isNegative
        ? (MAX_PRECISE
              / Precise(Sample<S>::min()))
            * Precise(Sample<T>::min() + isDithering)
        : (MAX_PRECISE
              / Precise(Sample<S>::max()))
            * Precise(Sample<T>::max() - isDithering);
}

template <typename T>
template <typename S>
T Sample<T>::convert(const Sample<S>& sample)
{
    S value = sample.value();

    if (Sample<T>::isFloat()) {
        if (Sample<S>::isFloat()) {
            return T(value);
        }

        return T(Precise(value) * ratio<S>(value < 0));
    }

    Precise precise;

    if (sizeof(T) < sizeof(S)) {
        precise = Dither::dither(Precise(value) * ratio<S>(value < 0, true));
    } else {
        precise = Precise(value) * ratio<S>(value < 0);
    }

    return T(Dither::round(precise));
}
}

#endif // SOUND_SAMPLE_TCC
