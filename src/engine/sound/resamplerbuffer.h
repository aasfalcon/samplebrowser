#ifndef RESAMPLERBUFFER_H
#define RESAMPLERBUFFER_H

#include "buffer.h"

namespace Sound {

template<class T>
class ResamplerBuffer : public Buffer<T> {
public:
    ResamplerBuffer(int frames = 0, int channels = 0);
    ResamplerBuffer(const Buffer<T> &buffer);

    void fromFloat(const float *source, int frames);
    void fromFloat(const Buffer<float> &source);
    void toFloat(Buffer<float> &dest) const;
    static void toFloat(Buffer<float> &dest, const T *source, int frames);
};

} // namespace Sound

#endif // RESAMPLERBUFFER_H
