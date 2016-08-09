#include <cstring>
#include "sample.h"
#include "buffer.h"
#include "converter.h"
#include "shared/iresampler.h"
#include "shared/server.h"

// BufferConverter

#define TEMPLATE(__dtype, __stype, __expr) \
    [](void *dest, const void *source, unsigned count) { \
        auto d = (Sound::__dtype *)dest; \
        auto s = (Sound::__stype *)source; \
        for (unsigned i = 0; i < count; i++, d++, s++) { \
            *d = static_cast<Sound::__dtype>(__expr); \
        } \
    },

#define FLOAT_FROM_FLOAT(__dbits, __sbits) \
    TEMPLATE(Float##__dbits, Float##__sbits, *s)

#define INT_FROM_FLOAT(__dbits, __sbits) \
    TEMPLATE(Int##__dbits, Float##__sbits, \
        static_cast<double>(*s) * Sound::MaxInt##__dbits)

#define FLOAT_FROM_INT(__dbits, __sbits) \
    TEMPLATE(Float##__dbits, Int##__sbits, \
        static_cast<double>(*s) / Sound::MaxInt##__sbits)

#define INT_FROM_INT(__dbits, __sbits) \
    [](void *dest, const void *source, unsigned count) { \
        auto d = (Sound::Int##__dbits *)dest; \
        auto s = (Sound::Int##__sbits *)source; \
        int shift = __sbits - __dbits; \
        if (shift > 0) { \
            for (unsigned i = 0; i < count; i++, d++, s++) { \
                *d = static_cast<int>(*s) >> shift; \
            } \
        } else { \
            for (unsigned i = 0; i < count; i++, d++, s++) { \
                *d = static_cast<int>(*s) << -shift; \
            } \
        } \
    },

#define MATRIX_ROW(__prefix, __dbits) { \
    __prefix##_FROM_FLOAT(__dbits, 32) \
    __prefix##_FROM_FLOAT(__dbits, 64) \
    __prefix##_FROM_INT(__dbits, 8) \
    __prefix##_FROM_INT(__dbits, 16) \
    __prefix##_FROM_INT(__dbits, 24) \
    __prefix##_FROM_INT(__dbits, 32) \
},

#define THE_MATRIX \
    MATRIX_ROW(FLOAT, 32) \
    MATRIX_ROW(FLOAT, 64) \
    MATRIX_ROW(INT, 8) \
    MATRIX_ROW(INT, 16) \
    MATRIX_ROW(INT, 24) \
    MATRIX_ROW(INT, 32)

const Converter::Convert
Converter::matrix[Sound::TypeCount][Sound::TypeCount] =
{
    THE_MATRIX
};

std::shared_ptr<IResampler> *resampler;

const unsigned
Converter::sizeMap[Sound::TypeCount] = {
    sizeof(Sound::Float32),
    sizeof(Sound::Float64),
    sizeof(Sound::Int8),
    sizeof(Sound::Int16),
    sizeof(Sound::Int24),
    sizeof(Sound::Int32),
};

void Converter::convert(void *dest, Sound::Type dtype,
                              const void *source, Sound::Type stype,
                              unsigned count)
{
    if (dtype == stype) {
        memcpy(dest, source, count * sizeMap[dtype]);
    } else {
        (*matrix[dtype][stype])(dest, source, count);
    }
}

void Converter::resample(void *dest, unsigned dframes,
                               const void *source, unsigned sframes,
                               unsigned channels, double ratio,
                               Sound::Type type, Sound::Quality quality)
{
    if (!resampler) {
        resampler = PLUGIN_FACTORY(IResampler);
    }

    if (type == Sound::TypeFloat32) {
        resampler->simple(static_cast<Sound::Float32 *>(dest), dframes,
                          static_cast<const Sound::Float32 *>(source), sframes,
                          channels, ratio, quality);
    } else {
        Buffer<Sound::Float32> dtemp(channels, dframes);
        Buffer<Sound::Float32> stemp;

        stemp.fromArray(source, type, channels, sframes);
        resampler->simple(dtemp.ptr(), dframes, stemp.ptr(), sframes,
                          channels, ratio, quality);
        convert(dest, type, dtemp.ptr(), Sound::TypeFloat32, dtemp.size());
    }
}
