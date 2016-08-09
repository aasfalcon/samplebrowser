#ifndef COMMON_H
#define COMMON_H

#include "shared/iconverter.h"

#define NSOUND_INSTANTIATE_SAMPLE_TYPES(__class) \
    namespace NSound { \
    template<> class __class<IConverter::float32>; \
    template<> class __class<IConverter::float64>; \
    template<> class __class<IConverter::int8>; \
    template<> class __class<IConverter::int16>; \
    template<> class __class<IConverter::int24>; \
    template<> class __class<IConverter::int32>; \
    }

#endif // COMMON_H

