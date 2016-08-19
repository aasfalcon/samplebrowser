#ifndef IRESAMPLER_H
#define IRESAMPLER_H

#include "../sound/object.h"
#include "interface.h"

struct IResampler : Interface {
    enum Quality {
        QualityRealtime,
        QualityFast = QualityRealtime,
        QualityMedium,
        QualityBest,

        QualityCount
    };

    virtual void init(Quality quality, unsigned channels, unsigned frames, double rate) = 0;
    virtual float* output() = 0;
    virtual unsigned process(const float* input, unsigned inputFramesMax) = 0;
    virtual void reset() = 0;
    virtual void setInputRate(double rate) = 0;
    virtual unsigned simple(float* dest, unsigned dframes,
        const float* source, unsigned sframes,
        unsigned channels, double ratio,
        Quality quality)
        = 0;

protected:
    virtual unsigned perform() = 0;
};

#endif // IRESAMPLER_H
