#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <memory>
#include <mutex>

#include "buffer.h"
#include "processor.h"
#include "shared/iresampler.h"

template<typename T>
class Resampler: public Processor<T>
{
public:
    Resampler();
    ~Resampler();

    void drop();

    template<typename S>
    void feed(const Buffer<S> &source, unsigned sampleRate);

    virtual void process();

protected:
    unsigned _bufferFrames;
    unsigned _position;
    unsigned _sourceFrames;

    virtual void init();

private:
    Buffer<Sound::Float32> _buffer;
    Buffer<Sound::Float32> _channelBuffer;
    std::shared_ptr<IResampler> _resampler;
    std::shared_ptr<char> _source;
    unsigned _sourceChannels;
    std::mutex _sourceMutex;
    unsigned _sourceRate;
    Sound::Type _sourceType;
};

#include "resampler.tcc"

#endif // RESAMPLER_H
