#include <samplerate.h>
#include <QDebug>

#include "error.h"
#include "resamplerbase.h"

using namespace Sound;

ResamplerPrivate::ResamplerPrivate(ResamplerPrivate::Quality quality)
    : _destChannels(0)
    , _quality(quality)
    , _state(0)
    , _stateChannels(0)
{
}

ResamplerPrivate::~ResamplerPrivate()
{
    if (_state) {
        src_delete((SRC_STATE *)_state);
    }
}

ResamplerPrivate::Quality ResamplerPrivate::quality() const
{
    return _quality;
}

double ResamplerPrivate::ratio() const
{
    if (_sourceRate != 0) {
        return _destRate / _sourceRate;
    }

    return 0;
}

void ResamplerPrivate::reset()
{
    src_reset((SRC_STATE *)_state);
}

int ResamplerPrivate::process(const float *source, float *dest)
{
    if (!_state || _stateChannels != _destChannels) {
        if (_state) {
            src_delete((SRC_STATE *)_state);
        }

        int error;
        _state = src_new(_quality, _stateChannels = _destChannels, &error);

        if (error) {
            ResamplerError exception;
            exception.message = QString(src_strerror(error));
            throw exception;
        }
    }

    SRC_DATA data = {
        (float *)source,
        dest,
        _sourceFrames,
        _destFrames,
        0, 0,
        0,
        ratio(),
    };

    int error = src_process((SRC_STATE *)_state, &data);

    if (error) {
        ResamplerError exception;
        exception.message = QString(src_strerror(error));
        throw exception;
    }

    int unused = (_destFrames - data.output_frames_gen) * _destChannels * sizeof(float);

    if (unused) {
        if (data.input_frames == data.input_frames_used) {
            qDebug() << _destFrames - data.output_frames_gen
                     << "unexpexted frame drops from resampler";
        }

        memset(dest + data.output_frames_gen, unused, 0);
    }

    return data.input_frames_used;
}
