#include <cstring>
#include <stdexcept>
#include <string>

#include "buffer.h"
#include "constframe.h"
#include "shared/iresampler.h"
#include "shared/log.h"
#include "shared/server.h"

using namespace Sound;

template <typename T>
void Buffer<T>::toInt24(void* dest)
{
    typedef std::uint8_t Int24Parts[3];
    union Int32Parts {
        std::int32_t i32;
        std::uint8_t i8[4];
    };

    const Int32Parts endiannessCheck = { 0x01020304 };
    const bool isBigEndian = 0x01 == endiannessCheck.i8[3];

    auto int24dest = reinterpret_cast<Int24Parts*>(dest);
    auto ptr = _samples.data();
    unsigned offset = unsigned(!isBigEndian);

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
    auto myBegin = begin();
    auto myEnd = end();

    if (myBegin - dbeg < 0 || dbeg - myEnd < 0
        || myBegin - dend < 0 || dend - myEnd < 0
        || dend - dbeg < 0) {
        std::string message = "Frame out of buffer bounds";
        LOG(ERROR, message);
        throw std::out_of_range(message);
    }

    std::memset(dbeg.ptr(), 0, dend.ptr() - dbeg.ptr());
}

SOUND_INSTANTIATE(Buffer);
