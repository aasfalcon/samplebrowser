#include <cstring>
#include <thread>

#include "resampler.h"
#include "shared/iresampler.h"
#include "shared/server.h"

#define PROCESSOR Resampler
#include "shortcuts.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Resampler<T>::Resampler()
    : _resampler(PLUGIN_FACTORY(IResampler))
{
    INITIALIZE;

    COMMAND(Start);
    COMMAND(Stop);

    PROPERTY(FeedFunc, Feed, nullptr);
    PROPERTY(ResamplerLibrary, Library, LibraryDefault);

    PARAMETER(IResampler::Quality, Quality, IResampler::QualityRealtime);
    PARAMETER(unsigned, SourceChannels, 0);
    PARAMETER(unsigned, SourceSampleRate, 0);
}

template <typename T>
Resampler<T>::~Resampler()
{
}

template <typename T>
void Resampler<T>::process()
{
    if (!_ring || _ring->isEmpty() || !this->runtime().channelsOutput) {
        return;
    }

    auto& source = _ring->pop();

    unsigned count = _resampler->process(source.cbegin().data(), source.frames());
    auto sptr = reinterpret_cast<const Sample<Float32>*>(_resampler->output());
    ConstFrame<Float32> sbeg(source.channels(), sptr);

    auto& out = this->output();
    auto silenceBegin = out.copyFrom(sbeg, sbeg + int(count));
    out.silence(silenceBegin, out.end());
}

template <typename T>
void Resampler<T>::commandInit()
{
    Silence<T>::commandInit();

    auto &info = this->runtime();
    _resampler->init(IResampler::QualityRealtime, info.channelsOutput,
        info.frames, info.sampleRate);
}

template <typename T>
void Resampler<T>::commandStart()
{
    unsigned schannels = this->get(Parameter::Resampler::SourceChannels);
    unsigned srate = this->get(Parameter::Resampler::SourceSampleRate);
    FeedFunc feed = this->property(Property::Resampler::Feed);

    _resampler->setInputRate(srate);
    _resampler->reset();

    unsigned bufferFrames = feedFrames(srate);
    _ring = std::make_shared<RingBuffer<InternalFormat> >(schannels, bufferFrames);

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
    auto &info = this->runtime();
    double ratio = double(sourceSampleRate) / double(info.sampleRate);
    return unsigned(ratio * double(info.frames));
}

INSTANTIATE;
