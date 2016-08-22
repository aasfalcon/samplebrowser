#include <cstring>
#include <thread>

#include "resampler.h"
#include "shared/iresampler.h"
#include "shared/server.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Resampler<T>::Resampler()
    : _resampler(PLUGIN_FACTORY(IResampler))
{
    SOUND_REGISTER_COMMAND(Resampler, Start);
    SOUND_REGISTER_COMMAND(Resampler, Stop);
}

template <typename T>
Resampler<T>::~Resampler()
{
}

template <typename T>
void Resampler<T>::process()
{
    Silence<T>::process();

    if (!_ring || _ring->isEmpty()) {
        return;
    }

    auto& source = _ring->pop();

    unsigned count = _resampler->process(source.cbegin().ptr(), source.frames());
    auto sptr = reinterpret_cast<const Sample<Float32>*>(_resampler->output());
    ConstFrame<Float32> sbeg(source.channels(), sptr);

    auto& buffer = this->buffer();
    buffer.copy(sbeg, sbeg + int(count));
}

template <typename T>
void Resampler<T>::commandInit()
{
    Silence<T>::commandInit();
    unsigned sampleRate = this->get(Property::Processor::SampleRate_unsigned);
    _resampler->init(IResampler::QualityRealtime, this->buffer().channels(),
        this->buffer().frames(), sampleRate);
}

template <typename T>
void Resampler<T>::commandStart()
{
    unsigned channels = this->get(Property::Resampler::SourceChannels_unsigned);
    unsigned srate = this->get(Property::Resampler::SourceSampleRate_unsigned);
    Feed feed = this->get(Property::Resampler::Callback_Sound_Processor_Resampler_Feed);

    _mutex.lock();
    _resampler->setInputRate(srate);
    _resampler->reset();

    unsigned bufferFrames = feedFrames(srate);
    _ring = std::make_shared<RingBuffer<InternalFormat> >(channels, bufferFrames);

    unsigned usleep = bufferFrames * 1000 * _ring->count() / srate / 3;
    _feeder = std::make_shared<RingFeeder<T, InternalFormat> >(*_ring);
    _feeder->start(feed, usleep);

    _mutex.unlock();
}

template <typename T>
void Resampler<T>::commandStop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _feeder.reset();
    _ring.reset();
}

template <typename T>
unsigned Resampler<T>::feedFrames(unsigned sourceSampleRate)
{
    unsigned sampleRate = this->get(Property::Processor::SampleRate_unsigned);
    double ratio = double(sourceSampleRate) / double(sampleRate);
    return unsigned(ratio * double(this->buffer().frames()));
}

SOUND_INSTANTIATE(Sound::Processor::Resampler);
