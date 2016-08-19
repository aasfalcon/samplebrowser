#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>

#include "buffer.h"
#include "constframe.h"
#include "shared/iresampler.h"
#include "shared/log.h"
#include "shared/server.h"

using namespace Sound;

typedef std::uint8_t Int24Parts[3];
union Int32Parts {
    std::int32_t i32;
    std::uint8_t i8[4];
};

inline bool isBigEndian() {
    static const Int32Parts endiannessCheck = { 0x01020304 };
    return 0x01 == endiannessCheck.i8[3];
}

template <typename T>
void Buffer<T>::fromInt24(const void *source)
{
    auto int24source = reinterpret_cast<const Int24Parts*>(source);
    auto ptr = _samples.data();
    unsigned offset = unsigned(!isBigEndian());

    for (unsigned i = 0; i < size(); i++) {
        const Int24Parts &s = int24source[i];
        Int32Parts d = { 0 };
        d.i8[0 + offset] = s[0];
        d.i8[1 + offset] = s[1];
        d.i8[2 + offset] = s[2];
        ptr[i] = Sample<Int32>(d.i32);
    }
}

template <typename T>
void Buffer<T>::toInt24(void* dest) const
{
    auto int24dest = reinterpret_cast<Int24Parts*>(dest);
    auto ptr = _samples.data();
    unsigned offset = unsigned(!isBigEndian());

    for (unsigned i = 0; i < size(); i++) {
        Sample<Int32> i32 = ptr[i];
        Int32Parts s = { i32 };
        Int24Parts& d = int24dest[i];
        d[0] = s.i8[0 + offset];
        d[1] = s.i8[1 + offset];
        d[2] = s.i8[2 + offset];
    }
}

template <typename T>
void Buffer<T>::reallocate(unsigned channels, unsigned frames)
{
    if (_channels != channels || _frames != frames) {
        if (_channels * _frames < channels * frames) {
            _samples.clear();
        }

        _samples.resize(channels * frames);
        _channels = channels;
        _frames = frames;
    }
}

template <typename T>
void Buffer<T>::resize(unsigned frames)
{
    if (_frames != frames) {
        _samples.resize(_channels * frames);
        _frames = frames;
    }
}

template <typename T>
Buffer<T> Buffer<T>::resample(unsigned destRate, unsigned sourceRate,
    IResampler::Quality quality)
{
    Buffer<T> buffer;
    buffer.resample(cbegin(), cend(), destRate, sourceRate, quality);
    return buffer;
}

template <typename T>
void Buffer<T>::resample(ConstFrame<T> sbeg, ConstFrame<T> send,
    unsigned destRate, unsigned sourceRate,
    IResampler::Quality quality)
{
    double ratio = double(destRate) / double(sourceRate);
    unsigned sframes = unsigned(send - sbeg);
    unsigned channels = sbeg.channels();
    unsigned dframes = unsigned(double(sframes) * ratio);
    auto resampler = PLUGIN_FACTORY(IResampler);

    if (this->type() == TypeFloat32) {
        Buffer<Float32> dtemp(channels, dframes);
        Buffer<Float32> stemp(sbeg, send);
        resampler->simple(dtemp.begin().ptr(), dframes,
            stemp.begin().ptr(), sframes,
            channels, ratio, quality);
        assign(dtemp.cbegin(), dtemp.cend());
    }
}

template <typename T>
void Buffer<T>::silence(Frame<T> dbeg, Frame<T> dend)
{
    assert(end() - dend >= 0);
    assert(dbeg - begin() >= 0);

    if (dbeg != dend) {
        std::size_t count = unsigned(dend - dbeg) * dbeg.channels() * sizeof(T);
        std::memset(dbeg.ptr(), 0, count);
    }
}

SOUND_INSTANTIATE(Buffer);
