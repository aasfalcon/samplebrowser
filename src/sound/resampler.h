#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <memory>
#include <mutex>
#include <thread>

#include "processor.h"
#include "ringbuffer.h"
#include "shared/iresampler.h"

template <typename T>
class Resampler : public Processor<T> {
public:
    typedef void (*Feeder)(RingBuffer<Sound::Float32>& ring, bool& isEnough, void *userData);

    Resampler();
    ~Resampler();

    void process();
    void start(unsigned channels, unsigned sampleRate, Feeder feeder, void *userData);
    void stop();

protected:
    std::mutex _mutex;

    unsigned feedFrames(unsigned sourceSampleRate);
    void init();

private:
    Feeder _feeder;
    bool _isEnough;
    std::shared_ptr<IResampler> _resampler;
    std::shared_ptr<RingBuffer<Sound::Float32> > _ring;
    unsigned _sampleRate;
    void *_userData;
};

SOUND_INSTANTIATION_DECLARE(Resampler);

#endif // RESAMPLER_H
