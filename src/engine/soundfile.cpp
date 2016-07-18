#include <sndfile.h>
#include <soxr.h>

#include "config.h"
#include "soundfile.h"

SoundFile::SoundFile()
    : _data()
    , _filename()
    , _info(new SF_INFO())
{
}

SoundFile::SoundFile(const QString &filename) {
    this->read(filename);
}

SoundFile::~SoundFile() {
    delete this->_info;
}

void SoundFile::read(const QString &filename) {
    this->_info->format = 0;
    this->_info->frames = 0;
    SNDFILE *handle = sf_open(filename.toUtf8(), SFM_READ, this->_info);

    if (!handle) {
        qDebug() << sf_strerror(handle);
        return;
    }

    this->_data.resize(this->channels() * this->frames());
    SoundFile::Sample *buffer = this->_data.data();
    int count = sf_readf_double(handle, buffer, this->frames());

    if (count != this->frames()) {
        qDebug() << "Count: " << count << ", should be: " << this->frames();
        this->_info->frames = count;
    }

    if (sf_error(handle)) {
        qDebug() << sf_strerror(handle);
        this->_info->frames = 0;
    }

    sf_close(handle);
}

void SoundFile::resample(int sampleRate) {
    if (this->sampleRate() != sampleRate) {
        double input_rate = (double)this->sampleRate();
        double output_rate = (double)sampleRate;
        int frames_resample = (int)(output_rate/input_rate*this->frames());
        Buffer data_resample(frames_resample * this->channels(), 0);
        size_t read, resampled;
        soxr_quality_spec_t quality = {
            24,     // precision
            50,     // phase_response
            0.913,  // passband_end
            1,      // stopband_begin
            0,      // reserved
            SOXR_HI_PREC_CLOCK // flags
        };
        soxr_io_spec_t io = {
            SOXR_FLOAT64,   // itype
            SOXR_FLOAT64,   // otype
            1,              // scale
            0,              // reserved
            0               // flags
        };
        soxr_runtime_spec_t runtime = {
            10,     // log2_min_dft_size
            17,     // log2_large_dft_size
            400,    // coef_size_kbytes
            1,      // num_threads
            0,      // reserved
            SOXR_COEF_INTERP_AUTO //flags
        };

        soxr_oneshot(input_rate, output_rate, this->channels(),
                     this->_data.data(), this->frames(), &read,
                     data_resample.data(), frames_resample, &resampled,
                     &io, &quality, &runtime);

        this->_data = data_resample;
        this->_info->samplerate = sampleRate;
        this->_info->frames = resampled;
    }
}

void SoundFile::resize(int frames) {
    if (this->frames() != frames) {
        this->_data.resize(this->channels() * frames);
        this->_info->frames = frames;
    }
}

bool SoundFile::isEmpty() const {
    return !this->frames();
}

int SoundFile::sampleRate() const {
    return this->_info->samplerate;
}

int SoundFile::channels() const {
    return this->_info->channels;
}

int SoundFile::frames() const {
    return this->_info->frames;
}
