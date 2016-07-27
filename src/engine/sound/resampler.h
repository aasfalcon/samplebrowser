#ifndef RESAMPLER_H
#define RESAMPLER_H

#include "resamplerbase.h"
#include "buffer.h"

namespace Sound {

template<class T>
class Resampler : public ResamplerPrivate
{
public:
    Resampler(Quality quality);

    void resample(const Buffer<T> &source, int sourceRate,
                  Buffer<T> &dest, int destRate);
};

} // namespace Sound

#endif // RESAMPLER_H
