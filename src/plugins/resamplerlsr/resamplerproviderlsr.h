#ifndef RESAMPLERPROVIDERLSR_H
#define RESAMPLERPROVIDERLSR_H

#include "common/resamplerprovider.h"

struct SRC_STATE_tag;

class ResamplerProviderLSR : public ResamplerProvider
{
public:
    virtual void fromFloat(void *output, Type outputType,
                           const float *input, unsigned count) const;
    virtual void initialize(Quality quality, unsigned channels, unsigned frames, double rate);
    virtual void toFloat(float *output, const void *input, Type inputType,
                         unsigned count) const;
    void reset();

private:
    SRC_STATE_tag *_state;

    void finalize();
    unsigned perform();
};

#endif // RESAMPLERPROVIDERLSR_H
