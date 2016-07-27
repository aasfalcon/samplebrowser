#ifndef IRESAMPLERPROVIDER_H
#define IRESAMPLERPROVIDER_H

#include "iprovider.h"

class IResamplerProvider : public IProvider
{
public:
    enum Quality {
        QualityFast,
        QualityMedium,
        QualityBest,
    };

    enum Type {
        TypeDouble,
        TypeFloat,
        TypeInt,
        TypeShort,
    };

    virtual void fromFloat(void *output, Type outptuType,
                           const float *input, unsigned count) const = 0;

    virtual void initialize(Quality quality, unsigned channels, unsigned frames, double rate) = 0;
    virtual float *output() = 0;
    virtual void process(const float *input, unsigned inputFramesMax, double rate) = 0;
    virtual void reset() = 0;

    virtual void toFloat(float *output, const void *input, Type inputType,
                         unsigned count) const = 0;

protected:
    virtual void finalize() = 0;
    virtual unsigned perform() = 0;
};

#endif // IRESAMPLERPROVIDER_H
