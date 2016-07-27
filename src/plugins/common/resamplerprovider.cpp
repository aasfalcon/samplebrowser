#include <climits>
#include <cstring>

#include "resamplerprovider.h"

#ifndef NULL
#define NULL 0
#endif

ResamplerProvider::ResamplerProvider()
    : _channels(0)
    , _frames(0)
    , _input(NULL)
    , _inputFramesMax(0)
    , _inputRate(0)
    , _isInitialized(false)
    , _output(NULL)
    , _outputRate(0)
{
}

ResamplerProvider::~ResamplerProvider()
{
    finalize();
}

void ResamplerProvider::fromFloat(void *output, IResamplerProvider::Type outputType, const float *input, unsigned count) const
{
    switch (outputType) {
    case TypeDouble: {
        double *dout = (double *)output;
        for (unsigned i = 0; i < count; i++, dout++, input++)
            *dout = *input;
        break;
    }
    case TypeInt: {
        int *iout = (int *)output;
        for (unsigned i = 0; i < count; i++, iout++, input++)
            *iout = *input * INT_MAX;
        break;
    }
    case TypeShort: {
        short *sout = (short *)output;
        for (unsigned i = 0; i < count; i++, sout++, input++)
            *sout = *input * SHRT_MAX;
        break;
    }
    default: // float
        memcpy(output, input, count * sizeof(float));
    }
}

void ResamplerProvider::initialize(IResamplerProvider::Quality,
                                   unsigned channels, unsigned frames,
                                   double rate)
{
    finalize();
    _channels = channels;
    _frames = frames;
    _outputRate = rate;
}

float *ResamplerProvider::output()
{
    return _output;
}

void ResamplerProvider::toFloat(float *output, const void *input, IResamplerProvider::Type inputType, unsigned count) const
{
    switch (inputType) {
    case TypeDouble: {
        double *din = (double *)input;
        for (unsigned i = 0; i < count; i++, din++, output++)
            *output = *din;
        break;
    }
    case TypeInt: {
        double *iin = (double *)input;
        for (unsigned i = 0; i < count; i++, iin++, output++)
            *output = ((float)*iin) / (float)INT_MAX;
        break;
    }
    case TypeShort: {
        double *sin = (double *)input;
        for (unsigned i = 0; i < count; i++, sin++, output++)
            *output = ((float)*sin) / (float)SHRT_MAX;
        break;
    }
    default: // float
        memcpy(output, input, count * sizeof(float));
    }
}

void ResamplerProvider::finalize()
{
    if (_isInitialized) {
        _isInitialized = false;
    }
}

void ResamplerProvider::process(const float *input,
                                unsigned inputFramesMax,
                                double inputRate)
{
    _input = input;
    _inputFramesMax = inputFramesMax;
    _inputRate = inputRate;
    perform();
}
