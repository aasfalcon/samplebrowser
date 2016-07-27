#include "config.h"
#include "error.h"
#include "realtimeresampler.h"

// RealtimeResampler

namespace Sound {

template<> RealtimeResampler<float>::RealtimeResampler(ResamplerPrivate::Quality quality, int outputFrames, int channels, int outputRate)
    : Resampler<float>(quality)
    , ResamplerBuffer<float>(outputFrames, channels)
{
    this->_destFrames = outputFrames;
    this->_destRate = outputRate;
    this->_destChannels = channels;
}

template<class T>
RealtimeResampler<T>::RealtimeResampler(ResamplerPrivate::Quality quality, int outputFrames, int channels, int outputRate)
    : Resampler<T>(quality)
    , ResamplerBuffer<T>(outputFrames, channels)
    , _floatInput(outputFrames * 8, channels)
    , _floatOutput(outputFrames, channels)
{
    this->_destFrames = outputFrames;
    this->_destRate = outputRate;
    this->_destChannels = channels;
}

template<>
int RealtimeResampler<float>::resample(const float *source, int max, int rate)
{
    this->_sourceRate = rate;
    this->_sourceFrames =
            this->_destFrames / this->_destRate * this->_sourceRate + WEE_RESAMPLE_TAIL;

    if (this->_sourceFrames > max) {
        this->_sourceFrames = max;
    }

    int used = 0;

    try {
        used = this->process(source, this->data());
    } catch (ResamplerError e) {
        qDebug() << e.toStrinng();
    }

    return used;
}

template<class T>
int RealtimeResampler<T>::resample(const T *source, int max, int rate)
{
    this->_sourceRate = rate;
    this->_sourceFrames = this->_destFrames / this->_destRate * this->_sourceRate + WEE_RESAMPLE_TAIL;

    if (this->_sourceFrames > max) {
        this->_sourceFrames = max;
    }

    try {
        RealtimeResampler::toFloat(this->_floatInput, source, this->_sourceFrames);
        this->process(this->_floatInput.data(), this->_floatOutput.data());
        this->fromFloat(this->_floatOutput);
    } catch (ResamplerError e) {
        qDebug() << e.toStrinng();
    }

    return this->_sourceFrames;
}

template class RealtimeResampler<short>;
template class RealtimeResampler<int>;
template class RealtimeResampler<float>;
template class RealtimeResampler<double>;

} // namespace Sound
