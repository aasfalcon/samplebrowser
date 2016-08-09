#ifndef RESAMPLER_H
#define RESAMPLER_H

#include <memory>
#include <mutex>
#include "processor.h"

struct IResampler;
template<typename T> class Buffer;

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
    virtual void init();

private:
    Buffer<Sound::Float32> _buffer;
    unsigned _bufferFrames;
    Buffer<Sound::Float32> _channelBuffer;
    unsigned _position;
    std::shared_ptr<IResampler> _resampler;
    std::shared_ptr<void *> _source;
    unsigned _sourceChannels;
    unsigned _sourceFrames;
    std::mutex _sourceMutex;
    unsigned _sourceRate;
    Sound::Type _sourceType;
};

#endif // RESAMPLER_H
