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

    int format = sfinfo.format;
    _info.format.major = FormatMajor(format & SF_FORMAT_TYPEMASK >> 16);
    _info.format.minor = FormatMinor(format & SF_FORMAT_SUBMASK);

    // major format
    SF_FORMAT_INFO sffi;
    sffi.format = format & SF_FORMAT_TYPEMASK;

    int errorNumber = sf_command(NULL, SFC_GET_FORMAT_INFO, &sffi, sizeof sffi);

    if (errorNumber) {
        ERROR_SNDFILE(errorNumber, "Can't get major format info");
    }

    _info.format.majorText = sffi.name;

    // minor format
    sffi.format = format & SF_FORMAT_SUBMASK;

    errorNumber = sf_command(NULL, SFC_GET_FORMAT_INFO, &sffi, sizeof sffi);

    if (errorNumber) {
        ERROR_SNDFILE(errorNumber, "Can't get minor format info");
    }

    _info.format.minorText = sffi.name;

    // strings
    for (int i = 0; i < StringEntryCount; i++) {
        const char *string = sf_get_string(_handle.get(), i + 1);

        if (!string) {
            errorNumber = sf_error(_handle.get());

            if (errorNumber != SF_ERR_NO_ERROR) {
                ERROR_SNDFILE(errorNumber, "Can't get string entry");
            }
        }

        _info.strings[i] = string;
    }

    // known chunks
    _info.chunkFlags = 0;
    int size;

    SF_BROADCAST_INFO bi;
    size = offsetof(SF_BROADCAST_INFO, coding_history);

    if (sf_command(_handle.get(), SFC_GET_BROADCAST_INFO, &bi, size)) {
        _info.chunkFlags |= 1 << ChunkBroadcastInfo;
        size += bi.coding_history_size;
        std::vector<ChunkData> buffer(unsigned(size), 0);

        if (!sf_command(_handle.get(), SFC_GET_BROADCAST_INFO, buffer.data(), size)) {
            errorNumber = sf_error(_handle.get());
            ERROR_SNDFILE(errorNumber, "Can't read broadcast info");
        }

        _chunks[ChunkBroadcastInfo] = buffer;
    }

    SF_CART_INFO ci;
    size = offsetof(SF_CART_INFO, tag_text);

    if (sf_command(_handle.get(), SFC_GET_CART_INFO, &ci, size)) {
        _info.chunkFlags |= 1 << ChunkCartInfo;
        size += ci.tag_text_size;
        std::vector<ChunkData> buffer(unsigned(size), 0);

        if (!sf_command(_handle.get(), SFC_GET_CART_INFO, buffer.data(), size)) {
            errorNumber = sf_error(_handle.get());
            ERROR_SNDFILE(errorNumber, "Can't read cart info");
        }

        _chunks[ChunkCartInfo] = buffer;
    }

    int count;

    if (sf_command(_handle.get(), SFC_GET_CUE_COUNT, &count, sizeof count)) {
        _info.chunkFlags |= 1 << ChunkCues;
        size = int(sizeof(SF_CUE_POINT)) * count;
        std::vector<ChunkData> buffer(unsigned(size), 0);

        if (!sf_command(_handle.get(), SFC_GET_CUE, buffer.data(), size)) {
            errorNumber = sf_error(_handle.get());
            ERROR_SNDFILE(errorNumber, "Can't read cue points");
        }

        _chunks[ChunkCues] = buffer;
    }

    SF_INSTRUMENT in;
    size = offsetof(SF_INSTRUMENT, loops);

    if (sf_command(_handle.get(), SFC_GET_INSTRUMENT, &in, size)) {
        _info.chunkFlags |= 1 << ChunkInstrument;
        size += int(sizeof in.loops[1]) * in.loop_count;
        std::vector<ChunkData> buffer(unsigned(size), 0);

        if (!sf_command(_handle.get(), SFC_GET_INSTRUMENT, buffer.data(), size)) {
            errorNumber = sf_error(_handle.get());
            ERROR_SNDFILE(errorNumber, "Can't read instrument data");
        }

        _chunks[ChunkInstrument] = buffer;
    }

    SF_LOOP_INFO li;
    size = sizeof(SF_LOOP_INFO);

    if (sf_command(_handle.get(), SFC_GET_LOOP_INFO, &li, size)) {
        _info.chunkFlags |= 1 << ChunkLoopInfo;
        std::vector<ChunkData> buffer(unsigned(size), 0);
        memcpy(buffer.data(), &li, size);
        _chunks[ChunkLoopInfo] = buffer;
    }

    // unknown chunks
    SF_CHUNK_INFO sfci;
    SF_CHUNK_ITERATOR *it = sf_get_chunk_iterator(_handle.get(), NULL);
    memset(&sfci, 0, sizeof sfci);

    while (it) {
        int errorNumber = sf_get_chunk_data(it, &sfci);

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Error reading chunk data");
        }

        std::string id(sfci.id, sfci.id_size);
        std::vector<ChunkData> buffer(sfci.datalen);
        memcpy(buffer.data(), sfci.data, int(sfci.datalen));
        _chunksRaw[id] = buffer;
    }
}

const IAudioFile::ChunkData *AudioFileSndfile::chunk(const char *id, unsigned *size)
{
    const std::vector<ChunkData> &result = _chunksRaw.at(id);
    *size = result.size();
    return result.data();
}

const IAudioFile::ChunkData *AudioFileSndfile::chunk(IAudioFile::ChunkType type, unsigned *size)
{
    const std::vector<ChunkData> &result = _chunks.at(type);
    *size = result.size();
    return result.data();
}

void AudioFileSndfile::setChunk(const char *id, unsigned size, const IAudioFile::ChunkData *data)
{
    if (_chunksFlushed) {
        ERROR("Attempt to set chunk after data write");
    }

    std::vector<ChunkData> buffer(size);
    memcpy(buffer.data(), data, int(size));
    _chunksRaw[id] = buffer;
}

void AudioFileSndfile::setChunk(IAudioFile::ChunkType type, unsigned size, const IAudioFile::ChunkData *data)
{
    if (_chunksFlushed) {
        ERROR("Attempt to set chunk after data write");
    }

    std::vector<ChunkData> buffer(size);
    memcpy(buffer.data(), data, int(size));
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

unsigned AudioFileSndfile::seek(int offset, IAudioFile::SeekWhence sw,
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

    sf_count_t result = sf_seek(_handle.get(), offset, whence);

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

std::string AudioFileSndfile::sndfileError(int errorNumber,
                                           const std::string &userMessage)
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
            ERROR_SNDFILE(errorNumber, "Unable to set broadcast info");
        }
    }

    if (_info.chunkFlags & (1 << ChunkCartInfo)) {
        buffer = _chunks[ChunkCartInfo];
        errorNumber = sf_command(_handle.get(), SFC_SET_CART_INFO, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to set cart info");
        }
    }

    if (_info.chunkFlags & (1 << ChunkCues)) {
        buffer = _chunks[ChunkCues];
        errorNumber = sf_command(_handle.get(), SFC_SET_CUE, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to set cues");
        }
    }

    if (_info.chunkFlags & (1 << ChunkInstrument)) {
        buffer = _chunks[ChunkInstrument];
        errorNumber = sf_command(_handle.get(), SFC_SET_INSTRUMENT, buffer.data(), int(buffer.size()));

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Unable to set instrument chunk");
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
            ERROR_SNDFILE(errorNumber, "Unable to set loop info");
        }
    }

    for (auto it = _chunksRaw.begin(); it != _chunksRaw.end(); ++it) {
        buffer = it->second;

        SF_CHUNK_INFO sfci;
        memset(&sfci, 0, sizeof sfci);
        strncpy(sfci.id, it->first.c_str(), sizeof sfci.id);
        sfci.id_size = it->first.length();
        sfci.data = buffer.data();
        sfci.datalen = buffer.size();

        errorNumber = sf_set_chunk(_handle.get(), &sfci);

        if (errorNumber) {
            ERROR_SNDFILE(errorNumber, "Error setting raw chunk");
        }
    }
}