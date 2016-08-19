#include <cstring>
#include <samplerate.h>
#include <stdexcept>

#include "resamplerlsr.h"

int ResamplerLSR::_types[IResampler::QualityCount] = {
    SRC_SINC_FASTEST, // QualityFast
    SRC_SINC_MEDIUM_QUALITY, // QualityMedium
    SRC_SINC_BEST_QUALITY, // QualityBest
};

ResamplerLSR::ResamplerLSR()
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

ResamplerLSR::~ResamplerLSR()
{
    if (_output) {
        delete _output;
    }

    if (_state) {
        src_delete(_state);
    }
}

void ResamplerLSR::init(IResampler::Quality quality, unsigned channels,
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

    _state = src_new(_types[quality], int(channels), &error);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    setInputRate(rate);
}

float* ResamplerLSR::output()
{
    return _output;
}

unsigned ResamplerLSR::perform()
{
    double ratio = _outputRate / _inputRate;
    SRC_DATA data = { const_cast<float*>(_input), _output,
        int(_inputFramesMax),
        int(_frames), 0, 0, 0, ratio };

    int error = src_process(_state, &data);

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    if (data.output_frames > data.output_frames_gen) {
        int offset = data.output_frames_gen * int(_channels);
        int frames = data.output_frames - data.output_frames_gen;
        std::memset(data.data_out + offset, 0, frames * _channels * sizeof(float));
    }

    return unsigned(data.input_frames_used);
}
unsigned ResamplerLSR::process(const float* input, unsigned inputFramesMax)
{
    _input = input;
    _inputFramesMax = inputFramesMax;
    return perform();
}

void ResamplerLSR::setInputRate(double rate)
{
    _inputRate = rate;
}

void ResamplerLSR::reset()
{
    if (_state) {
        int error = src_reset(_state);

        if (error) {
            throw std::runtime_error(src_strerror(error));
        }
    }
}

unsigned ResamplerLSR::simple(float* dest, unsigned dframes,
    const float* source, unsigned sframes,
    unsigned channels,
    double ratio, IResampler::Quality quality)
{
    SRC_DATA data = {
        const_cast<float*>(source), dest,
        long(sframes), long(dframes),
        0, 0,
        1,
        ratio
    };

    int error = src_simple(&data, _types[quality], int(channels));

    if (error) {
        throw std::runtime_error(src_strerror(error));
    }

    return unsigned(data.output_frames_gen);
}
