#include <samplerate.h>
#include "resamplerbuffer.h"

namespace Sound {

template<class T>
ResamplerBuffer<T>::ResamplerBuffer(int frames, int channels)
    : Buffer<T>::Buffer(frames, channels)
{
}

template<class T>
ResamplerBuffer<T>::ResamplerBuffer(const Buffer<T> &buffer)
    : Buffer<T>::Buffer(buffer)
{
}

template<class T>
void ResamplerBuffer<T>::fromFloat(const Buffer<float> &source)
{
    fromFloat(source.data(), source.frames());
}

template<> void ResamplerBuffer<float>::fromFloat(const Buffer<float> &source) {
    (*this) = source;
}

template<> void ResamplerBuffer<short>::fromFloat(const float *source, int frames)
{
    framesResize(frames);
    src_float_to_short_array(source, data(), length());
}

template<> void ResamplerBuffer<int>::fromFloat(const float *source, int frames)
{
    framesResize(frames);
    src_float_to_int_array(source, data(), length());
}

template<> void ResamplerBuffer<float>::fromFloat(const float *source, int frames)
{
    framesResize(frames);
    memcpy(data(), source, length() * sizeof(float));
}

template<> void ResamplerBuffer<double>::fromFloat(const float *source, int frames)
{
    framesResize(frames);
    double *dest = data();
    int count = length();

    for (int i = 0; i < count; i++, source++, dest++) {
        *dest = *source;
    }
}

template<class T>
void ResamplerBuffer<T>::toFloat(Buffer<float> &dest) const
{
    toFloat(dest, this->data(), this->frames());
}

template <>
void ResamplerBuffer<short>::toFloat(Buffer<float> &dest, const short *source, int frames)
{
    dest.framesResize(frames);
    src_short_to_float_array(source, dest.data(), dest.length());
}

template <>
void ResamplerBuffer<int>::toFloat(Buffer<float> &dest, const int *source, int frames)
{
    dest.framesResize(frames);
    src_int_to_float_array(source, dest.data(), dest.length());
}

template <>
void ResamplerBuffer<float>::toFloat(Buffer<float> &dest, const float *source, int frames)
{
    dest.framesResize(frames);
    memcpy(dest.data(), source, dest.length() * sizeof(float));
}

template <>
void ResamplerBuffer<double>::toFloat(Buffer<float> &dest, const double *source, int frames)
{
    dest.framesResize(frames);
    float *pdest = dest.data();
    int count = dest.length();

    for (int i = 0; i < count; i++, source++, pdest++) {
        *pdest = *source;
    }
}

template class ResamplerBuffer<short>;
template class ResamplerBuffer<int>;
template class ResamplerBuffer<float>;
template class ResamplerBuffer<double>;

} // namespace Sound
