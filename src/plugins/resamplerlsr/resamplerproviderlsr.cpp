#include <cstring>
#include <stdexcept>
#include <samplerate.h>

#include "resamplerproviderlsr.h"

int ResamplerProviderLSR::_types[Sound::QualityCount] = {
    SRC_SINC_FASTEST,           // QualityFast
    SRC_SINC_MEDIUM_QUALITY,    // QualityMedium
    SRC_SINC_BEST_QUALITY,      // QualityBest
};

ResamplerProviderLSR::ResamplerProviderLSR()
    : IResampler()
    , _channels(0)
    , _frames(0)
    , _input(NULL)
    , _inputFramesMax(0)
    , _inputRate(0)
    , _output(NULL)
    , _outputRate(0)
    , _state(NULL)
{
}

ResamplerProviderLSR::~ResamplerProviderLSR()
{
    if (_output) {
        delete _output;
    }

    if (_state) {
        src_delete(_state);
    }
}

void ResamplerProviderLSR::init(Sound::Quality quality, unsigned channels,
                                unsigned frames, double rate)
{
    _channels = channels;
    _frames = frames;

    if (_output) {
        delete _output;
    }

    _output = new float[frames * channels];
    _outputRate = rate;

    int error = 0;

    if (_state) {
        src_delete(_state);
    }

    _state = src_new(_types[quality], channels, &error);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    setInputRate(rate);
}

float *ResamplerProviderLSR::output()
{
    return _output;
}

unsigned ResamplerProviderLSR::perform()
{
    double ratio = _outputRate / _inputRate;
    SRC_DATA data = { (float *)_input, _output, (int)_inputFramesMax,
                      (int)_frames, 0, 0, 0, ratio };

    int error = src_process((SRC_STATE *)_state, &data);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    if (data.output_frames > data.output_frames_gen) {
        memset(data.data_out + data.output_frames_gen * _channels, 0,
               (data.output_frames - data.output_frames_gen) * _channels * sizeof(float));
    }

    return data.input_frames_used;
}
unsigned ResamplerProviderLSR::process(const float *input, unsigned inputFramesMax)
{
    _input = input;
    _inputFramesMax = inputFramesMax;
    return perform();
}

void ResamplerProviderLSR::setInputRate(double rate)
{
    _inputRate = rate;
}

void ResamplerProviderLSR::reset()
{
    if (_state) {
        int error = src_reset(_state);

        if (error) {
            throw std::runtime_error(src_strerror(error));
        }
    }
}

unsigned ResamplerProviderLSR::simple(float *dest, unsigned dframes,
                                      const float *source, unsigned sframes,
                                      unsigned channels,
                                      double ratio, Sound::Quality quality)
{
    SRC_DATA data = {
        (float *)source, dest,
        (long)sframes, (long)dframes,
        0, 0,
        1,
        ratio
    };

    int error = src_simple(&data, _types[quality], channels);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    return data.output_frames_gen;
}
