#include <cstring>
#include <thread>

#define PROCESSOR Resampler

#include "resampler.h"
#include "shared/iresampler.h"
#include "shared/server.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Resampler<T>::Resampler()
    : _resampler(PLUGIN_FACTORY(IResampler))
{
    COMMAND(Start);
    COMMAND(Stop);

    PROPERTY(FeedFunc, Feed, nullptr);
    PROPERTY(ResamplerLibrary, Library, LibraryDefault);
    PROPERTY(IResampler::Quality, Quality, IResampler::QualityRealtime);
    PROPERTY(unsigned, SourceChannels, 0);
    PROPERTY(unsigned, SourceSampleRate, 0);
}

template <typename T>
Resampler<T>::~Resampler()
{
}

template <typename T>
void Resampler<T>::process()
{
    if (!_ring || _ring->isEmpty()) {
        return;
    }

    auto& source = _ring->pop();

    unsigned count = _resampler->process(source.cbegin().ptr(), source.frames());
    auto sptr = reinterpret_cast<const Sample<Float32>*>(_resampler->output());
    ConstFrame<Float32> sbeg(source.channels(), sptr);

    auto& buffer = this->buffer();
    auto silenceBegin = buffer.copy(sbeg, sbeg + int(count));
    buffer.silence(silenceBegin, buffer.end());
}

template <typename T>
void Resampler<T>::commandInit()
{
    Silence<T>::commandInit();
    unsigned sampleRate = this->get(Parameter::Processor::SampleRate);
    _resampler->init(IResampler::QualityRealtime, this->output().channels(),
        this->output().frames(), sampleRate);
}

template <typename T>
void Resampler<T>::commandStart()
{
    unsigned channels = this->property(Property::Resampler::SourceChannels);
    unsigned srate = this->property(Property::Resampler::SourceSampleRate);
    FeedFunc feed = this->property(Property::Resampler::Feed);

    _resampler->setInputRate(srate);
    _resampler->reset();

    unsigned bufferFrames = feedFrames(srate);
    _ring = std::make_shared<RingBuffer<InternalFormat> >(channels, bufferFrames);

    unsigned usleep = bufferFrames * 1000 * _ring->count() / srate / 3;
    _feeder = std::make_shared<RingFeeder<T, InternalFormat> >(*_ring);
    _feeder->start(feed, usleep);
}

template <typename T>
void Resampler<T>::commandStop()
{
    _feeder.reset();
    _ring.reset();
}

template <typename T>
unsigned Resampler<T>::feedFrames(unsigned sourceSampleRate)
{
    unsigned sampleRate = this->get(Parameter::Processor::SampleRate);
    double ratio = double(sourceSampleRate) / double(sampleRate);
    return unsigned(ratio * double(this->buffer().frames()));
}

INSTANTIATE;
