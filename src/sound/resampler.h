#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <memory>
#include <mutex>
#include <thread>

#include "processor.h"
#include "ringbuffer.h"
#include "shared/iresampler.h"

namespace Sound {

template <typename T>
class Resampler : public Processor<T> {
public:
    typedef void (*Feeder)(RingBuffer<Float32>& ring,
        bool& isEnough, void* userData);

    Resampler();
    ~Resampler();

    void process();
    void start(unsigned channels, unsigned sampleRate,
        Feeder feeder, void* userData);
    void stop();

protected:
    unsigned feedFrames(unsigned sourceSampleRate);
    void init();

private:
    Feeder _feeder;
    bool _isEnough;
    std::mutex _mutex;
    std::shared_ptr<IResampler> _resampler;
    std::shared_ptr<RingBuffer<Float32> > _ring;
    unsigned _sampleRate;
    void* _userData;
};

SOUND_INSTANTIATION_DECLARE(Resampler);
}

#endif // RESAMPLER_H
