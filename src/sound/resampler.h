#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <memory>
#include <mutex>
#include <thread>

#include "processor.h"
#include "ringbuffer.h"
#include "ringfeeder.h"
#include "shared/iresampler.h"

namespace Sound {

template <typename T>
class Resampler : public Processor<T> {
public:
    typedef Float32 InternalFormat;
    typedef typename RingFeeder<T, InternalFormat>::FeedFunc FeedFunc;

    Resampler();
    ~Resampler();

    void process();
    void start(unsigned channels, unsigned sampleRate, FeedFunc feed);
    void stop();

protected:
    unsigned feedFrames(unsigned sourceSampleRate);
    void init();

private:
    std::shared_ptr<RingFeeder<T, InternalFormat> > _feeder;
    std::mutex _mutex;
    std::shared_ptr<IResampler> _resampler;
    std::shared_ptr<RingBuffer<InternalFormat> > _ring;
    unsigned _sampleRate;
};

SOUND_INSTANTIATION_DECLARE(Resampler);
}

#endif // RESAMPLER_H
