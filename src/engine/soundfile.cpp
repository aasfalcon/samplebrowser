#include <sndfile.h>
#include <soxr.h>

#include <QTime>
#include <QFileInfo>

#include "config.h"
#include "soundfile.h"

QMap<int, QString> SoundFile::_Map;

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
    int count = sf_readf_float(handle, buffer, this->frames());

    if (count != this->_info->frames) {
        qDebug() << "Count: " << count << ", should be: " << this->_info->frames;
        this->_info->frames = count;
    }

    if (sf_error(handle)) {
        qDebug() << sf_strerror(handle);
        this->_info->frames = 0;
    }

    sf_close(handle);
    this->_filename = filename;
}

SoundFile::TextInfo SoundFile::readInfo(const QString &filename) {
    SoundFile temp;
    SF_INFO *info = temp._info;
    info->format = 0;
    SNDFILE *handle = sf_open(filename.toUtf8(), SFM_READ, info);

    if (!handle) {
        qDebug() << sf_strerror(handle);
        std::exit(1);
    }

    sf_close(handle);

    temp._filename = filename;
    return temp.info();
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
            SOXR_FLOAT32,   // itype
            SOXR_FLOAT32,   // otype
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

SoundFile::TextInfo SoundFile::info() const
{
    // duration
    int msec = this->_info->frames * 1000 / this->_info->samplerate;
    QTime time = QTime(0, 0).addMSecs(msec);
    QString format = time.hour() > 0 || time.minute() > 0 ?
                "hh:mm:ss.zzz" : "s.zzz's'";
    QString duration = time.toString(format);

    // channels
    QString channels;

    if (this->_info->channels > 2) {
        channels = QString::number(this->_info->channels);
    } else {
        channels = this->_info->channels > 1 ? "Stereo" : "Mono";
    }

    // file size
    double size = QFileInfo(this->_filename).size();
    const char* units[] = {" B", " KiB", " MiB", " GiB"};
    int index = 0;
    for (; size > 1024; index++, size /= 1024);
    QString fileSize = QString::number(size, 'f', 2) + units[index];

    TextInfo result = {
        QFileInfo(this->_filename).fileName(),
        this->_GetFormatString(this->_info->format & SF_FORMAT_TYPEMASK),
        channels,
        this->_GetFormatString(this->_info->format & SF_FORMAT_SUBMASK),
        duration,
        QString::number(this->_info->samplerate) + " Hz",
        fileSize,
    };

    return result;
}

QString SoundFile::_GetFormatString(int key)
{
    if (!SoundFile::_Map.count()) {
        // Major formats (from libsndfile)
        SoundFile::_Map[SF_FORMAT_WAV] = "Microsoft WAV (LE)";
        SoundFile::_Map[SF_FORMAT_AIFF] = "Apple/SGI AIFF (BE)";
        SoundFile::_Map[SF_FORMAT_AU] = "Sun/NeXT AU (BE)";
        SoundFile::_Map[SF_FORMAT_RAW] = "RAW PCM data";
        SoundFile::_Map[SF_FORMAT_PAF] = "Ensoniq PARIS file";
        SoundFile::_Map[SF_FORMAT_SVX] = "Amiga IFF / SVX8 / SV16";
        SoundFile::_Map[SF_FORMAT_NIST] = "Sphere NIST";
        SoundFile::_Map[SF_FORMAT_VOC] = "VOC files";
        SoundFile::_Map[SF_FORMAT_IRCAM] = "Berkeley/IRCAM/CARL";
        SoundFile::_Map[SF_FORMAT_W64] = "Sonic Foundry's 64 bit RIFF/WAV";
        SoundFile::_Map[SF_FORMAT_MAT4] = "Matlab (tm) V4.2 / GNU Octave 2.0";
        SoundFile::_Map[SF_FORMAT_MAT5] = "Matlab (tm) V5.0 / GNU Octave 2.1";
        SoundFile::_Map[SF_FORMAT_PVF] = "Portable Voice";
        SoundFile::_Map[SF_FORMAT_XI] = "Fasttracker 2 Extended Instrument";
        SoundFile::_Map[SF_FORMAT_HTK] = "HMM Tool Kit";
        SoundFile::_Map[SF_FORMAT_SDS] = "Midi Sample Dump Standard";
        SoundFile::_Map[SF_FORMAT_AVR] = "Audio Visual Research";
        SoundFile::_Map[SF_FORMAT_WAVEX] = "MS WAVE with WAVEFORMATEX";
        SoundFile::_Map[SF_FORMAT_SD2] = "Sound Designer 2";
        SoundFile::_Map[SF_FORMAT_FLAC] = "FLAC lossless file";
        SoundFile::_Map[SF_FORMAT_CAF] = "Core Audio File";
        SoundFile::_Map[SF_FORMAT_WVE] = "Psion WVE";
        SoundFile::_Map[SF_FORMAT_OGG] = "Xiph OGG container";
        SoundFile::_Map[SF_FORMAT_MPC2K] = "Akai MPC 2000 sampler";
        SoundFile::_Map[SF_FORMAT_RF64] = "RF64 WAV file";

        // Subtypes
        SoundFile::_Map[SF_FORMAT_PCM_S8] = "8 bit PCM";
        SoundFile::_Map[SF_FORMAT_PCM_16] = "16 bit PCM";
        SoundFile::_Map[SF_FORMAT_PCM_24] = "24 bit PCM";
        SoundFile::_Map[SF_FORMAT_PCM_32] = "32 bit PCM";

        SoundFile::_Map[SF_FORMAT_PCM_U8] = "Unsigned 8 bit PCM";

        SoundFile::_Map[SF_FORMAT_FLOAT] = "32 bit float IEEE";
        SoundFile::_Map[SF_FORMAT_DOUBLE] = "64 bit float IEEE";

        SoundFile::_Map[SF_FORMAT_ULAW] = "U-Law encoded";
        SoundFile::_Map[SF_FORMAT_ALAW] = "A-Law encoded";
        SoundFile::_Map[SF_FORMAT_IMA_ADPCM] = "IMA ADPCM";
        SoundFile::_Map[SF_FORMAT_MS_ADPCM] = "Microsoft ADPCM";

        SoundFile::_Map[SF_FORMAT_GSM610] = "GSM 6.10";
        SoundFile::_Map[SF_FORMAT_VOX_ADPCM] = "Oki Dialogic ADPCM";

        SoundFile::_Map[SF_FORMAT_G721_32] = "32kbs G721 ADPCM";
        SoundFile::_Map[SF_FORMAT_G723_24] = "24kbs G723 ADPCM";
        SoundFile::_Map[SF_FORMAT_G723_40] = "40kbs G723 ADPCM";

        SoundFile::_Map[SF_FORMAT_DWVW_12] = "12 bit Delta Width Variable Word";
        SoundFile::_Map[SF_FORMAT_DWVW_16] = "16 bit Delta Width Variable Word";
        SoundFile::_Map[SF_FORMAT_DWVW_24] = "24 bit Delta Width Variable Word";
        SoundFile::_Map[SF_FORMAT_DWVW_N] = "N bit Delta Width Variable Word";

        SoundFile::_Map[SF_FORMAT_DPCM_8] = "8 bit differential PCM (XI only)";
        SoundFile::_Map[SF_FORMAT_DPCM_16] = "16 bit differential PCM (XI only)";

        SoundFile::_Map[SF_FORMAT_VORBIS] = "Xiph Vorbis";

        SoundFile::_Map[SF_FORMAT_ALAC_16] = "16 bit ALAC";
        SoundFile::_Map[SF_FORMAT_ALAC_20] = "20 bit ALAC";
        SoundFile::_Map[SF_FORMAT_ALAC_24] = "24 bit ALAC";
        SoundFile::_Map[SF_FORMAT_ALAC_32] = "32 bit ALAC";
    }

    return SoundFile::_Map[key];
}
