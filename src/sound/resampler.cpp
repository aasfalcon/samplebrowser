#include <cstring>
#include <thread>

#include "resampler.h"
#include "shared/server.h"

template <typename T>
Resampler<T>::Resampler()
    : _feeder(nullptr)
    , _isEnough(false)
{
}

template <typename T>
Resampler<T>::~Resampler()
{
}

template <typename T>
void Resampler<T>::process()
{
    if (!_mutex.try_lock()) {
        return;
    }

    if (!_ring) {
        _mutex.unlock();
        return;
    }

    if (_ring->isEmpty()) {
        _ring.reset();
        _mutex.unlock();
        return;
    }

    try {
        auto& buffer = _ring->pop();

        if (!_isEnough && _ring->isHalfEmpty()) {
            std::thread([this]() { (*_feeder)(*_ring, _isEnough, _userData); });
        }

        unsigned count = _resampler->process(buffer.cbegin().ptr(), buffer.frames());
        auto sptr = reinterpret_cast<const Sample<Sound::Float32>*>(_resampler->output());
        ConstFrame<Sound::Float32> sbeg(buffer.channels(), sptr);

        auto& out = *this->out();
        auto tail = out.copy(sbeg, sbeg + int(count));
        out.silence(tail, out.end());
    } catch (...) {
        _mutex.unlock();
        throw;
    }

    _mutex.unlock();
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

    _ring = std::make_shared<RingBuffer<Sound::Float32> >(channels,
        feedFrames(sampleRate));
    (*feeder)(*_ring, _isEnough, _userData = userData);
    _mutex.unlock();
}

template <typename T>
void Resampler<T>::stop()
{
    _mutex.lock();
    _ring.reset();
    _mutex.unlock();
}

template <typename T>
unsigned Resampler<T>::feedFrames(unsigned sourceSampleRate)
{
    double ratio = double(sourceSampleRate) / double(this->sampleRate());
    return unsigned(ratio * double(this->out()->frames()));
}

template <typename T>
void Resampler<T>::init()
{
    auto& out = *this->out();
    _resampler->init(Sound::QualityRealtime, out.channels(),
        out.frames(), this->sampleRate());
}

SOUND_INSTANTIATE(Resampler);
