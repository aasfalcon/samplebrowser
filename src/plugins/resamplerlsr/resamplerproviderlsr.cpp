#include <cstring>

#include "resamplerproviderlsr.h"

#include <samplerate.h>

void ResamplerProviderLSR::fromFloat(void *output,
                                     IResamplerProvider::Type outputType,
                                     const float *input,
                                     unsigned count) const
{
    switch (outputType) {
    case TypeInt:
        src_float_to_int_array(input, (int *)output, count);
        break;

    case TypeShort:
        src_float_to_short_array(input, (short *)output, count);
        break;

    default:
        ResamplerProvider::fromFloat(output, outputType, input, count);
    }
}

void ResamplerProviderLSR::initialize(IResamplerProvider::Quality quality,
                                      unsigned channels, unsigned frames,
                                      double rate)
{
    ResamplerProvider::initialize(quality, channels, frames, rate);
    int error = 0;
    int types[] = {
        SRC_SINC_FASTEST,           // QualityFast
        SRC_SINC_MEDIUM_QUALITY,    // QualityMedium
        SRC_SINC_BEST_QUALITY,      // QualityBest
    };

    _state = src_new(types[quality], channels, &error);

    if (error) {
        throw src_strerror(error);
    }
}

void ResamplerProviderLSR::toFloat(float *output, const void *input,
                                   IResamplerProvider::Type inputType,
                                   unsigned count) const
{
    switch (inputType) {
    case TypeInt:
        src_int_to_float_array((const int *)input, output, count);
        break;

    case TypeShort:
        src_short_to_float_array((const short *)input, output, count);
        break;

    default:
        ResamplerProvider::toFloat(output, input, inputType, count);
    }
}

void ResamplerProviderLSR::reset()
{
    if (_state) {
        int error = src_reset(_state);

        if (error) {
            throw src_strerror(error);
        }
    }
}

void ResamplerProviderLSR::finalize()
{
    if (_isInitialized) {
        if (_state) {
            _state = src_delete(_state);
        }

        ResamplerProvider::finalize();
    }
}

unsigned ResamplerProviderLSR::perform()
{
    double ratio = _outputRate / _inputRate;
    int inputFrames = (_frames + 1.0) / _outputRate * _inputRate;

    SRC_DATA data = {
        (float *)_input,
        _output,
        inputFrames,
        (int)_frames,
        0, 0,
        0,
        ratio,
    };

    int error = src_process((SRC_STATE *)_state, &data);

    if (error) {
        throw src_strerror(error);
    }

    unsigned unused =
            (_frames - data.output_frames_gen) * _channels * sizeof(float);

    if (unused) {
        memset(_output + data.output_frames_gen, 0, unused);
    }

    return data.input_frames_used;
}
