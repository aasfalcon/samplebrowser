#include <cstring>
#include <thread>

#include "resampler.h"
#include "shared/iresampler.h"
#include "shared/server.h"

using namespace Sound;

template <typename T>
Resampler<T>::Resampler()
    : _feeder(nullptr)
    , _isEnough(false)
    , _resampler(PLUGIN_FACTORY(IResampler))
{
}

template <typename T>
Resampler<T>::~Resampler()
{
}

template <typename T>
void Resampler<T>::process()
{
    if (!_ring || _ring->isEmpty()) {
        if (_ring) {
            _ring.reset();
        }

        return;
    }

    auto& buffer = _ring->pop();

    if (!_isEnough && _ring->isHalfEmpty()) {
        (*_feeder)(*_ring, _isEnough, _userData);
    }

    unsigned count = _resampler->process(buffer.cbegin().ptr(), buffer.frames());
    auto sptr = reinterpret_cast<const Sample<Float32>*>(_resampler->output());
    ConstFrame<Float32> sbeg(buffer.channels(), sptr);

    auto& out = this->out();
    auto silenceBegin = out.copy(sbeg, sbeg + int(count));
    out.silence(silenceBegin, out.end());
}

template <typename T>
void Resampler<T>::start(unsigned channels, unsigned sampleRate,
    Resampler<T>::Feeder feeder, void* userData)
{
    _mutex.lock();
    _feeder = feeder;
    _sampleRate = sampleRate;
    _resampler->setInputRate(_sampleRate);
    _resampler->reset();

    _ring = std::make_shared<RingBuffer<Float32> >(channels,
        feedFrames(sampleRate));
    _mutex.unlock();
    (*feeder)(*_ring, _isEnough, _userData = userData);
}

template <typename T>
void Resampler<T>::stop()
{
    std::lock_guard<std::mutex> lock(_mutex);
    _ring.reset();
}

template <typename T>
unsigned Resampler<T>::feedFrames(unsigned sourceSampleRate)
{
    double ratio = double(sourceSampleRate) / double(this->sampleRate());
    return unsigned(ratio * double(this->out().frames()));
}

template <typename T>
void Resampler<T>::init()
{
    _resampler->init(IResampler::QualityRealtime, this->out().channels(),
        this->out().frames(), this->sampleRate());
}

SOUND_INSTANTIATE(Resampler);
