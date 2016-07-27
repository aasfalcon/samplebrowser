#ifndef RESAMPLERBASE_H
#define RESAMPLERBASE_H

namespace Sound {

class ResamplerPrivate
{
public:
    enum Quality {
        QualityBest = 0,
        QualityMedium,
        QualityFast
    };

    ResamplerPrivate(Quality quality);
    ~ResamplerPrivate();

    Quality quality() const;
    double ratio() const;
    void reset();

protected:
    int _destChannels;
    int _destFrames;
    double _destRate;
    Quality _quality;
    int _sourceFrames;
    double _sourceRate;
    void *_state;
    int _stateChannels;

    int process(const float *source, float *dest);
};

} // namespace Sound

#endif // RESAMPLERBASE_H
