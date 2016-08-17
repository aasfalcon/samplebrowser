#include <algorithm>
#include <cstring>
#include <cstddef>
#include <stdexcept>
#include <sstream>

#include "sound/converter.h"
#include "shared/log.h"
#include "audiofilesndfile.h"

const unsigned LOGINFO_MAX_SIZE = 2048;

#define ERROR(what) \
    std::string message = what; \
    LOG(ERROR, what); \
    throw std::runtime_error(what);

#define ERROR_SNDFILE(number, what) \
    ERROR(sndfileError(number, what));

std::map<int, IAudioFile::FormatMajor> AudioFileSndfile::_majorMap = {
    {SF_FORMAT_WAV,     IAudioFile::MajorWAV},
    {SF_FORMAT_AIFF,    IAudioFile::MajorAIFF},
    {SF_FORMAT_AU,      IAudioFile::MajorAU},
    {SF_FORMAT_RAW,     IAudioFile::MajorRAW},
    {SF_FORMAT_PAF,     IAudioFile::MajorPAF},
    {SF_FORMAT_SVX,     IAudioFile::MajorSVX},
    {SF_FORMAT_NIST,    IAudioFile::MajorNIST},
    {SF_FORMAT_VOC,     IAudioFile::MajorVOC},
    {SF_FORMAT_IRCAM,   IAudioFile::MajorIRCAM},
    {SF_FORMAT_W64,     IAudioFile::MajorW64},
    {SF_FORMAT_MAT4,    IAudioFile::MajorMAT4},
    {SF_FORMAT_MAT5,    IAudioFile::MajorMAT5},
    {SF_FORMAT_PVF,     IAudioFile::MajorPVF},
    {SF_FORMAT_XI,      IAudioFile::MajorXI},
    {SF_FORMAT_HTK,     IAudioFile::MajorHTK},
    {SF_FORMAT_SDS,     IAudioFile::MajorSDS},
    {SF_FORMAT_AVR,     IAudioFile::MajorAVR},
    {SF_FORMAT_WAVEX,   IAudioFile::MajorWAVEX},
    {SF_FORMAT_SD2,     IAudioFile::MajorSD2},
    {SF_FORMAT_FLAC,    IAudioFile::MajorFLAC},
    {SF_FORMAT_CAF,     IAudioFile::MajorCAF},
    {SF_FORMAT_WVE,     IAudioFile::MajorWVE},
    {SF_FORMAT_OGG,     IAudioFile::MajorOGG},
    {SF_FORMAT_MPC2K,   IAudioFile::MajorMPC2K},
    {SF_FORMAT_RF64,    IAudioFile::MajorRF64},
};

std::map<int, IAudioFile::FormatMinor> AudioFileSndfile::_minorMap = {
    {SF_FORMAT_PCM_S8,      IAudioFile::MinorPCM_S8},
    {SF_FORMAT_PCM_16,      IAudioFile::MinorPCM_16},
    {SF_FORMAT_PCM_24,      IAudioFile::MinorPCM_24},
    {SF_FORMAT_PCM_32,      IAudioFile::MinorPCM_32},

    {SF_FORMAT_PCM_U8,      IAudioFile::MinorPCM_U8},

    {SF_FORMAT_FLOAT,       IAudioFile::MinorFloat},
    {SF_FORMAT_DOUBLE,      IAudioFile::MinorDouble},

    {SF_FORMAT_ULAW,        IAudioFile::MinorULAW},
    {SF_FORMAT_ALAW,        IAudioFile::MinorALAW},
    {SF_FORMAT_IMA_ADPCM,   IAudioFile::MinorIMA_ADPCM},
    {SF_FORMAT_MS_ADPCM,    IAudioFile::MinorMS_ADPCM},

    {SF_FORMAT_GSM610,      IAudioFile::MinorGSM610},
    {SF_FORMAT_VOX_ADPCM,   IAudioFile::MinorVOX_ADPCM},

    {SF_FORMAT_G721_32,     IAudioFile::MinorG721_32k},
    {SF_FORMAT_G723_24,     IAudioFile::MinorG723_24k},
    {SF_FORMAT_G723_40,     IAudioFile::MinorG723_40k},

    {SF_FORMAT_DWVW_12,     IAudioFile::MinorDWVW_12},
    {SF_FORMAT_DWVW_16,     IAudioFile::MinorDWVW_16},
    {SF_FORMAT_DWVW_24,     IAudioFile::MinorDWVW_24},
    {SF_FORMAT_DWVW_N,      IAudioFile::MinorDWVW_N},

    {SF_FORMAT_DPCM_8,      IAudioFile::MinorDPCM_8},
    {SF_FORMAT_DPCM_16,     IAudioFile::MinorDPCM_16},

    {SF_FORMAT_VORBIS,      IAudioFile::MinorVorbis},

    {SF_FORMAT_ALAC_16,     IAudioFile::MinorALAC_16},
    {SF_FORMAT_ALAC_20,     IAudioFile::MinorALAC_20},
    {SF_FORMAT_ALAC_24,     IAudioFile::MinorALAC_24},
    {SF_FORMAT_ALAC_32,     IAudioFile::MinorALAC_32},
};

AudioFileSndfile::AudioFileSndfile()
    : IAudioFile()
{

}

AudioFileSndfile::~AudioFileSndfile()
{

}

void AudioFileSndfile::close()
{
    if (_handle) {
        if ((_mode == ModeWrite || _mode == ModeReadWrite) && !_flushed) {
            flush();
        }

        _handle.reset();
    }
}

void AudioFileSndfile::flush()
{
    if (!_handle || _mode != ModeWrite || _mode != ModeReadWrite) {
        ERROR("Attempt to write file not opened for writing.");
    }

    sf_write_sync(_handle.get());
}

IAudioFile::FormatText &AudioFileSndfile::formatText(
        const IAudioFile::Format &format)
{
    _formatText.reset(new FormatText());

    int sfFormat = formatConvert(format);

    SF_FORMAT_INFO fi;
    fi.format = sfFormat & SF_FORMAT_TYPEMASK;
    int error = sf_command(NULL, SFC_GET_FORMAT_INFO, &fi, sizeof fi);

    if (error) {
        ERROR_SNDFILE(error, "Can't get major format info");
    }

    _formatText->majorText = fi.name;

    fi.format = sfFormat & SF_FORMAT_SUBMASK;
    error = sf_command(NULL, SFC_GET_FORMAT_INFO, &fi, sizeof fi);

    if (error) {
        ERROR_SNDFILE(error, "Can't get minor format info");
    }

    _formatText->minorText = fi.name;
    return *_formatText;
}

const IAudioFile::SimpleFormats &AudioFileSndfile::formats()
{
    if (!_simpleFormats) {
        _simpleFormats = std::make_shared<SimpleFormats>();

        sf_command(nullptr, SFC_GET_SIMPLE_FORMAT_COUNT,
                   &_simpleFormats->count, sizeof _simpleFormats->count);

        _simpleFormatsData.clear();
        SF_FORMAT_INFO fi;

        for (unsigned i = 0; i < _simpleFormats->count; i++) {
            fi.format = int(i);
            int error = sf_command(NULL, SFC_GET_SIMPLE_FORMAT, &fi, sizeof fi);

            if (error) {
                ERROR_SNDFILE(error, "Can't get simple format info");
            }

            SimpleFormats::Formats sff;
            sff.description = fi.name;
            sff.extension = fi.extension;
            sff.format = formatConvert(fi.format);
            _simpleFormatsData.push_back(sff);
        }

        _simpleFormats->formats = _simpleFormatsData.data();
    }

    return *_simpleFormats;
}

void AudioFileSndfile::open(const char *filename, IAudioFile::Mode mode)
{
    _mode = mode;
    _path = filename;

    if (ModeClosed == mode) {
        return;
    }

    static int sfmode[ModeCount] = {
        0,
        SFM_READ,
        SFM_WRITE,
        SFM_RDWR,
    };

    SF_INFO sfinfo;
    auto psndfile = sf_open(filename, sfmode[mode], &sfinfo);
    auto deleter = [](SNDFILE *handle) { sf_close(handle); };
    _handle = std::shared_ptr<SNDFILE>(psndfile, deleter);

    if (!_handle) {
        int errorNumber = sf_error(NULL);
        ERROR_SNDFILE(errorNumber, "Error opening file handle");
    }

    if (mode == ModeWrite) {
        return;
    }

    _info.channels = unsigned(sfinfo.channels);
    _info.frames = unsigned(sfinfo.frames);
    _info.sampleRate = unsigned(sfinfo.samplerate);
    _info.seekable = bool(sfinfo.seekable);
    _info.format = formatConvert(sfinfo.format);

    switch (int(_info.format.minor)) {
    case MinorDPCM_8:
    case MinorPCM_S8:
    case MinorPCM_U8:
        _info.sampleType = Sound::TypeInt8;
        break;

    case MinorALAC_20:
    case MinorALAC_24:
    case MinorDWVW_24:
    case MinorPCM_24:
        _info.sampleType = Sound::TypeInt24;
        break;

    case MinorALAC_32:
    case MinorPCM_32:
        _info.sampleType = Sound::TypeInt32;
        break;

    case MinorDWVW_N:
    case MinorFloat:
    case MinorVorbis:
        // TODO: move following subformats to AudioFileMPG123
    case MinorMPEG1_0_DualChannel:
    case MinorMPEG1_0_JointStereo:
    case MinorMPEG1_0_Mono:
    case MinorMPEG1_0_Stereo:
    case MinorMPEG2_0_DualChannel:
    case MinorMPEG2_0_JointStereo:
    case MinorMPEG2_0_Mono:
    case MinorMPEG2_0_Stereo:
    case MinorMPEG2_5_DualChannel:
    case MinorMPEG2_5_JointStereo:
    case MinorMPEG2_5_Mono:
    case MinorMPEG2_5_Stereo:
        _info.sampleType = Sound::TypeFloat32;
        break;

    case MinorDouble:
        _info.sampleType = Sound::TypeFloat64;
        break;

    default:
        _info.sampleType = Sound::TypeInt16;
    }

    // unknown chunks
    SF_CHUNK_ITERATOR *it = sf_get_chunk_iterator(_handle.get(), NULL);
    SF_CHUNK_INFO sfci;
    memset(&sfci, 0, sizeof sfci);
    _rawChunksData.clear();

    while (it) {
        int errorNumber = sf_get_chunk_data(it, &sfci);

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Error reading chunk data");
        }

        std::string id(sfci.id, sfci.id_size);
        std::vector<ChunkData> data;
        data.assign(reinterpret_cast<ChunkData *>(sfci.data),
                    reinterpret_cast<ChunkData *>(sfci.data) + sfci.datalen);
        _rawChunksData.push_back({id, data});
    }
}

IAudioFile::RawChunk *AudioFileSndfile::rawChunks(unsigned *count)
{
    // valid until setRawChunks called
    *count = _rawChunksData.size();

    _rawChunks.clear();

    for (auto it = _rawChunksData.cbegin(); it != _rawChunksData.cend(); ++it) {
        _rawChunks.push_back({ it->id.c_str(), it->data.data(), it->data.size() });
    }

    return _rawChunks.data();
}

const IAudioFile::ChunkData *AudioFileSndfile::chunk(IAudioFile::ChunkType type,
                                                     unsigned *size)
{
    switch (type) {
    case ChunkBroadcastInfo: {
        SF_BROADCAST_INFO bi;
        int bytes = offsetof(SF_BROADCAST_INFO, coding_history);

        if (sf_command(_handle.get(), SFC_GET_BROADCAST_INFO, &bi, bytes)) {
            _info.chunkFlags |= 1 << ChunkBroadcastInfo;
            bytes += bi.coding_history_size;
            std::vector<ChunkData> buffer(unsigned(bytes), 0);

            if (!sf_command(_handle.get(), SFC_GET_BROADCAST_INFO, buffer.data(), bytes)) {
                int errorNumber = sf_error(_handle.get());
                ERROR_SNDFILE(errorNumber, "Can't read broadcast info");
            }

            _chunks[ChunkBroadcastInfo] = buffer;
        }

        break;
    };

    case ChunkCartInfo: {
        SF_CART_INFO ci;
        int bytes = offsetof(SF_CART_INFO, tag_text);

        if (sf_command(_handle.get(), SFC_GET_CART_INFO, &ci, bytes)) {
            _info.chunkFlags |= 1 << ChunkCartInfo;
            bytes += ci.tag_text_size;
            std::vector<ChunkData> buffer(unsigned(bytes), 0);

            if (!sf_command(_handle.get(), SFC_GET_CART_INFO, buffer.data(), bytes)) {
                int errorNumber = sf_error(_handle.get());
                ERROR_SNDFILE(errorNumber, "Can't read cart info");
            }

            _chunks[ChunkCartInfo] = buffer;
        }

        break;
    };

    case ChunkCues: {
        int count;

        if (sf_command(_handle.get(), SFC_GET_CUE_COUNT, &count, sizeof count)) {
            _info.chunkFlags |= 1 << ChunkCues;
            int bytes = int(sizeof(SF_CUE_POINT)) * count;
            std::vector<ChunkData> buffer(unsigned(bytes), 0);

            if (!sf_command(_handle.get(), SFC_GET_CUE, buffer.data(), bytes)) {
                int errorNumber = sf_error(_handle.get());
                ERROR_SNDFILE(errorNumber, "Can't read cue points");
            }

            _chunks[ChunkCues] = buffer;
        }

        break;
    };

    case ChunkInstrument: {
        SF_INSTRUMENT in;
        int bytes = offsetof(SF_INSTRUMENT, loops);

        if (sf_command(_handle.get(), SFC_GET_INSTRUMENT, &in, bytes)) {
            _info.chunkFlags |= 1 << ChunkInstrument;
            bytes += int(sizeof in.loops[1]) * in.loop_count;
            std::vector<ChunkData> buffer(unsigned(bytes), 0);

            if (!sf_command(_handle.get(), SFC_GET_INSTRUMENT, buffer.data(), bytes)) {
                int errorNumber = sf_error(_handle.get());
                ERROR_SNDFILE(errorNumber, "Can't read instrument data");
            }

            _chunks[ChunkInstrument] = buffer;
        }

        break;
    };

    case ChunkLoopInfo: {
        SF_LOOP_INFO li;
        int bytes = sizeof(SF_LOOP_INFO);

        if (sf_command(_handle.get(), SFC_GET_LOOP_INFO, &li, bytes)) {
            _info.chunkFlags |= 1 << ChunkLoopInfo;
            std::vector<ChunkData> buffer(unsigned(bytes), 0);
            memcpy(buffer.data(), &li, bytes);
            _chunks[ChunkLoopInfo] = buffer;
        }

        break;
    };

    default: {
        std::string message = "Unknown chunk type";
        LOG(ERROR, message);
        throw std::out_of_range(message);
    };

    };

    try {
        const std::vector<ChunkData> &result = _chunks.at(type);
        *size = result.size();
        return result.data();
    } catch (std::out_of_range) {
        *size = 0;
        return nullptr;
    }
}

void AudioFileSndfile::setChunk(IAudioFile::ChunkType type, unsigned size, const IAudioFile::ChunkData *data)
{
    if (_chunksFlushed) {
        ERROR("Attempt to write chunk after data write");
    }

    std::vector<ChunkData> buffer;
    buffer.assign(data, data + size);
    _chunks[type] = buffer;
}

const IAudioFile::FileInfo *AudioFileSndfile::fileInfo() const
{
    return &_info;
}

void AudioFileSndfile::setFileInfo(const IAudioFile::FileInfo *value)
{
    _info = *value;
}

void AudioFileSndfile::setRawChunks(IAudioFile::RawChunk *chunks, unsigned count)
{
    if (_chunksFlushed) {
        ERROR("Attempt to write chunk after data write");
    }

    _rawChunksData.clear();
    _rawChunks.clear();

    for (unsigned i = 0; i < count; i++) {
        std::string id = chunks[i].id;
        std::vector<ChunkData> data;
        data.assign(chunks[i].data, chunks[i].data + chunks[i].size);
        _rawChunksData.push_back({ id, data });
    }
}

const char *AudioFileSndfile::string(IAudioFile::StringEntry entry)
{
    const char *result = sf_get_string(_handle.get(), entry);

    if (!result) {
        int errorNumber = sf_error(_handle.get());

        if (errorNumber != SF_ERR_NO_ERROR) {
            ERROR_SNDFILE(errorNumber, "Can't read string entry");
        }
    }

    return result;
}

void AudioFileSndfile::setString(IAudioFile::StringEntry entry, const char *value)
{
    int error = sf_set_string(_handle.get(), entry, value);

    if (error != SF_ERR_NO_ERROR) {
        ERROR_SNDFILE(error, "Can't write string entry");
    }
}

void AudioFileSndfile::setProgressCallback(IAudioFile::ProgressCallback callback)
{
    _progress = callback;
}

unsigned AudioFileSndfile::read(void *buffer, unsigned frames)
{
    if (!_handle || !(_mode == ModeRead || _mode == ModeReadWrite)) {
        ERROR("Attempt to read file not opened for reading.");
    }

    sf_count_t count;

    switch (int(_info.sampleType)) {
    case Sound::TypeFloat32:
        count = sf_readf_float(_handle.get(), static_cast<Sound::Float32 *>(buffer), frames);
        break;
    case Sound::TypeFloat64:
        count = sf_readf_double(_handle.get(), static_cast<Sound::Float64 *>(buffer), frames);
        break;
    case Sound::TypeInt8:
        count = sf_read_raw(_handle.get(), buffer, frames * _info.channels);
        break;
    case Sound::TypeInt16:
        count = sf_readf_short(_handle.get(), static_cast<Sound::Int16 *>(buffer), frames);
        break;
    case Sound::TypeInt24: {
        auto temp = std::vector<Sound::Float32>(frames * _info.channels);
        count = sf_readf_float(_handle.get(), temp.data(), frames);
        Converter::instance().convert(
                    buffer, Sound::TypeInt24,
                    temp.data(), Sound::TypeFloat32,
                    frames * _info.channels);
        break;
    }
    case Sound::TypeInt32:
        count = sf_readf_int(_handle.get(), static_cast<Sound::Int32 *>(buffer), frames);
        break;
    default:
        ERROR("Unsupported sample type");
    }

    int errorNumber = sf_error(_handle.get());

    if (errorNumber) {
        ERROR_SNDFILE(errorNumber, "Error while reading file");
    }

    return unsigned(count);
}

unsigned AudioFileSndfile::seek(int pos, IAudioFile::SeekWhence sw,
                                IAudioFile::SeekType st)
{
    if (!_info.seekable || !_handle) {
        ERROR("File not seekable or handle not open");
    }

    static int whenceMap[3] = {
        SF_SEEK_CUR,
        SF_SEEK_END,
        SF_SEEK_SET,
    };

    int whence = whenceMap[sw];

    if (SeekTypeRead == st) {
        whence |= SFM_READ;
    } else if (SeekTypeWrite == st) {
        whence |= SFM_WRITE;
    }

    sf_count_t result = sf_seek(_handle.get(), pos, whence);

    if (result < 0) {
        int errorNumber = sf_error(_handle.get());
        ERROR_SNDFILE(errorNumber, "Unable to seek file");
    }

    return unsigned(result);
}

void AudioFileSndfile::write(const void *buffer, unsigned frames)
{
    if (!_handle || _mode != ModeWrite || _mode != ModeReadWrite) {
        ERROR("Attempt to write file not opened for writing.");
    }

    flushChunks();
    sf_count_t count;

    switch (int(_info.sampleType)) {
    case Sound::TypeFloat32:
        count = sf_writef_float(_handle.get(), static_cast<const Sound::Float32 *>(buffer), frames);
        break;
    case Sound::TypeFloat64:
        count = sf_writef_double(_handle.get(), static_cast<const Sound::Float64 *>(buffer), frames);
        break;
    case Sound::TypeInt8:
        count = sf_write_raw(_handle.get(), buffer, frames * _info.channels);
        break;
    case Sound::TypeInt16:
        count = sf_writef_short(_handle.get(), static_cast<const Sound::Int16 *>(buffer), frames);
        break;
    case Sound::TypeInt24: {
        auto temp = std::vector<Sound::Float32>(frames * _info.channels);
        Converter::instance().convert(
                    temp.data(), Sound::TypeFloat32,
                    buffer, Sound::TypeInt24,
                    frames * _info.channels);
        count = sf_writef_float(_handle.get(), temp.data(), frames);
        break;
    }
    case Sound::TypeInt32:
        count = sf_writef_int(_handle.get(), static_cast<const Sound::Int32 *>(buffer), frames);
        break;
    default:
        ERROR("Unsupported sample type");
    }
}

IAudioFile::Format AudioFileSndfile::formatConvert(int sfformat) const
{
    IAudioFile::Format result;

    result.major = _majorMap[sfformat & SF_FORMAT_TYPEMASK];
    result.minor = _minorMap[sfformat & SF_FORMAT_SUBMASK];

    return result;
}

int AudioFileSndfile::formatConvert(const IAudioFile::Format &format) const
{
    int result = 0;

    for (auto it = _majorMap.cbegin(); it != _majorMap.cend(); ++it) {
        if (it->second == format.major) {
            result = it->first;
            break;
        }
    }

    for (auto it = _minorMap.cbegin(); it != _minorMap.cend(); ++it) {
        if (it->second == format.minor) {
            result |= it->first;
            break;
        }
    }

    return result;
}

std::string AudioFileSndfile::sndfileError(int errorNumber,
                                           const std::string &userMessage) const
{
    const std::string libraryMessage = sf_error_number(errorNumber);
    char logInfo[LOGINFO_MAX_SIZE];
    sf_command(_handle.get(), SFC_GET_LOG_INFO, logInfo, LOGINFO_MAX_SIZE);
    LOG(INFO,
        "Library error detailed information" << "\n\n"
        << "Sound file: " << _path << '\n'
        << "Library reports: " << libraryMessage << '\n'
        << "Library version: " << sf_version_string() << '\n'
        << "Library log follows:\n" << logInfo
        );

    return userMessage + ": " + libraryMessage;
}

void AudioFileSndfile::flushChunks()
{
    if (_chunksFlushed) {
        return;
    }

    int errorNumber;
    std::vector<ChunkData> buffer;

    if (_info.chunkFlags & (1 << ChunkBroadcastInfo)) {
        buffer = _chunks[ChunkBroadcastInfo];
        errorNumber = sf_command(_handle.get(), SFC_SET_BROADCAST_INFO, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to write broadcast info");
        }
    }

    if (_info.chunkFlags & (1 << ChunkCartInfo)) {
        buffer = _chunks[ChunkCartInfo];
        errorNumber = sf_command(_handle.get(), SFC_SET_CART_INFO, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to write cart info");
        }
    }

    if (_info.chunkFlags & (1 << ChunkCues)) {
        buffer = _chunks[ChunkCues];
        errorNumber = sf_command(_handle.get(), SFC_SET_CUE, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to write cues");
        }
    }

    if (_info.chunkFlags & (1 << ChunkInstrument)) {
        buffer = _chunks[ChunkInstrument];
        errorNumber = sf_command(_handle.get(), SFC_SET_INSTRUMENT, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to write instrument chunk");
        }
    }

    if (_info.chunkFlags & (1 << ChunkLoopInfo)) {
        buffer = _chunks[ChunkLoopInfo];
        auto li = reinterpret_cast<SF_LOOP_INFO *>(buffer.data());

        unsigned flags = 0;

        if (li->loop_mode == SF_LOOP_FORWARD) {
            flags |= 0x01;
        }

        if (!li->root_key) {
            flags |= 0x02;
        }

        struct {
            uint32_t flags;
            uint16_t rootNote;
            uint16_t wReserved;
            float fReserved;
            uint32_t beats;
            uint16_t time_sig_den;
            uint16_t time_sig_num;
            float bpm;
        } acid = {
            flags,
            uint16_t(li->root_key),
            0x8000,
            .0,
            uint16_t(li->num_beats),
            uint16_t(li->time_sig_den),
            uint16_t(li->time_sig_num),
            float(li->bpm),
        };

        SF_CHUNK_INFO sfci;
        memset(&sfci, 0, sizeof sfci);
        strcpy(sfci.id, "acid");
        sfci.id_size = 4;
        sfci.data = &acid;
        sfci.datalen = sizeof acid;

        errorNumber = sf_set_chunk(_handle.get(), &sfci);

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to write loop info");
        }
    }

    for (auto it = _rawChunksData.begin(); it != _rawChunksData.end(); ++it) {
        SF_CHUNK_INFO sfci;
        memset(&sfci, 0, sizeof sfci);
        strncpy(sfci.id, it->id.c_str(), sizeof sfci.id);
        sfci.id_size = it->id.length();
        sfci.data = it->data.data();
        sfci.datalen = it->data.size();

        errorNumber = sf_set_chunk(_handle.get(), &sfci);

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Error writing raw chunk");
        }
    }
}
