#include <mpg123.h>
#include <sstream>
#include <stdexcept>

#include "audiofilempg123.h"
#include "shared/log.h"

#define MPG123_ERROR_N(__message, __number) \
    RUNTIME_ERROR(mpgError(__message, __number));

#define MPG123_ERROR(__message) \
    RUNTIME_ERROR(mpgError(__message));

AudioFileMPG123::AudioFileMPG123()
{
    mpg123_init();
}

AudioFileMPG123::~AudioFileMPG123()
{
    mpg123_exit();
}

void AudioFileMPG123::close()
{
    if (_mpghandle) {
        mpg123_delete(_mpghandle);
        _mpghandle = NULL;
    }

    AudioFileSndfile::close();
}

void AudioFileMPG123::open(const char* filename, IAudioFile::Mode mode)
{
    _path = filename;
    std::string ext;
    std::string::size_type index = _path.rfind('.');

    if (index != std::string::npos) {
        ext = _path.substr(index + 1);
    }

    _mpgmode = ext == "mp3" || ext == "mp2" || ext == "mp1";

    if (!_mpgmode) {
        AudioFileSndfile::open(filename, mode);
        return;
    }

    if (mode == ModeWrite || mode == ModeReadWrite) {
        RUNTIME_ERROR("Writing MPEG is not supported");
    }

    int errorNumber;
    _mpghandle = mpg123_new(NULL, &errorNumber);

    if (!_mpghandle) {
        MPG123_ERROR_N("Error creating handle", errorNumber);
    }

    if (MPG123_OK != mpg123_open(_mpghandle, filename)) {
        MPG123_ERROR("Error opening file");
    }

    if (MPG123_OK != mpg123_scan(_mpghandle)) {
        MPG123_ERROR("Error scanning file");
    }

    int length = mpg123_length(_mpghandle);

    if (MPG123_ERR == length) {
        MPG123_ERROR("Can't get file length");
    }

    _info.frames = unsigned(length);

    mpg123_frameinfo fi;

    if (MPG123_OK != mpg123_info(_mpghandle, &fi)) {
        MPG123_ERROR("Can't get frame info");
    }

    static FormatMajor layerMap[3] = {
        MajorMP1,
        MajorMP2,
        MajorMP3,
    };

    _info.format.major = layerMap[fi.layer - 1];

    static FormatMinor verMap[3][4] = {
        { MinorMPEG1_0_Stereo, MinorMPEG1_0_JointStereo,
            MinorMPEG1_0_DualChannel, MinorMPEG1_0_Mono },
        { MinorMPEG2_0_Stereo, MinorMPEG2_0_JointStereo,
            MinorMPEG2_0_DualChannel, MinorMPEG2_0_Mono },
        { MinorMPEG2_5_Stereo, MinorMPEG2_5_JointStereo,
            MinorMPEG2_5_DualChannel, MinorMPEG2_5_Mono },
    };

    _info.format.minor = verMap[fi.version][fi.mode];

    static CompressionType brMap[3] = {
        CompressionCBR,
        CompressionVBR,
        CompressionABR,
    };

    _info.compression.type = brMap[fi.vbr];
    _info.compression.bitrate = unsigned(MPG123_CBR == fi.vbr ? fi.bitrate : fi.abr_rate);
    _info.channels = fi.mode == MPG123_M_MONO ? 1 : 2;
    _info.chunkFlags = 0;
    _info.seekable = true;
    _info.sampleRate = unsigned(fi.rate);
    _info.sampleType = Sound::TypeFloat32;
}

unsigned AudioFileMPG123::read(void* buffer, Sound::Type type, unsigned frames)
{
    if (!_mpgmode) {
        return AudioFileSndfile::read(buffer, type, frames);
    }

    int size = int(frames * _info.channels * sizeof(Sound::Float32));
    std::size_t sizeRead;
    unsigned char* ptr = static_cast<unsigned char*>(buffer);
    int errorNumber = mpg123_read(_mpghandle, ptr, size, &sizeRead);

    if (MPG123_OK != errorNumber) {
        MPG123_ERROR_N("Error reading MPEG stream", errorNumber);
    }

    return unsigned(sizeRead);
}

unsigned AudioFileMPG123::seek(int pos, IAudioFile::SeekWhence sw, IAudioFile::SeekType st)
{
    if (!_mpgmode) {
        return AudioFileSndfile::seek(pos, sw, st);
    }

    static int whenceMap[3] = {
        SEEK_CUR,
        SEEK_END,
        SEEK_SET,
    };

    int result = mpg123_seek(_mpghandle, pos, whenceMap[sw]);

    if (result < 0) {
        MPG123_ERROR_N("Error seeking MPEG stream", result);
    }

    return unsigned(result);
}

std::string AudioFileMPG123::mpgError(const std::string& userMessage,
    int errorNumber)
{
    std::ostringstream message;
    message << userMessage << '\n'
            << "File path: " << _path << '\n'
            << "mpg123 message: ";

    if (errorNumber) {
        message << mpg123_plain_strerror(errorNumber);
    } else {
        message << mpg123_strerror(_mpghandle);
    }

    return message.str();
}
