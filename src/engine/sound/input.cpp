#include "libsndfile/src/sndfile.h"

#include "error.h"
#include "input.h"

using namespace Sound;

Input::Input(const QString &path)
    : File(path, 0, 0, 0, 0)
{
    SF_INFO info;
    _handle = sf_open(path.toUtf8(), SFM_READ, &info);

    if (!_handle) {
        errorLibrary("Error opening file for reading");
    }

    _path = path;
    _channels = info.channels;
    _frames = info.frames;
    _format = info.format;
    _sampleRate = info.samplerate;

    memset(_chunks, 0, sizeof _chunks);
}

Input::~Input()
{
    for (int i = 0; i < ChunkTypeCount; i++) {
        if (_chunks[i]) {
            delete _chunks[i];
        }
    }
}

bool Input::wavexAmbisonic()
{
    return (bool)sf_command(_handle, SFC_WAVEX_GET_AMBISONIC, NULL, 0);
}

void Input::loadChunk(File::ChunkType type)
{
    int size;
    QByteArray buffer;
    Chunk *chunk = 0;

    switch (type) {
    case ChunkBroadcastInfo:
        SF_BROADCAST_INFO bi;
        size = offsetof(SF_BROADCAST_INFO, coding_history);

        if (sf_command(_handle, SFC_GET_BROADCAST_INFO, &bi, size)) {
            buffer.resize(size + bi.coding_history_size);

            if (!sf_command(_handle, SFC_GET_BROADCAST_INFO, buffer.data(), buffer.length())) {
                throw errorLibrary("Can't read broadcast info");
            }
            chunk = new BroadcastInfo;
            chunk->fromRaw(buffer);
        }

        break;

    case ChunkCartInfo:
        SF_CART_INFO ci;
        size = offsetof(SF_CART_INFO, tag_text);

        if (sf_command(_handle, SFC_GET_CART_INFO, &ci, size)) {
            buffer.resize(size + ci.tag_text_size);

            if (!sf_command(_handle, SFC_GET_BROADCAST_INFO, buffer.data(), buffer.length())) {
                throw errorLibrary("Can't read cart info");
            }

            chunk = new CartInfo;
            chunk->fromRaw(buffer);
        }
        break;

    case ChunkCues:
        if (sf_command(_handle, SFC_GET_CUE_COUNT, &size, sizeof size)) {
            buffer.resize(size * sizeof(SF_CUE_POINT));

            if (!sf_command(_handle, SFC_GET_CUE, buffer.data(), buffer.length())) {
                throw errorLibrary("Can't read cue points");
            }

            chunk = new Cues;
            chunk->fromRaw(buffer);
        }
        break;

    case ChunkInstrument:
        SF_INSTRUMENT in;
        size = offsetof(SF_INSTRUMENT, loops);

        if (sf_command(_handle, SFC_GET_INSTRUMENT, &in, size)) {
            buffer.resize(size + in.loop_count * sizeof in.loops[1]);

            if (!sf_command(_handle, SFC_GET_INSTRUMENT, buffer.data(), buffer.size())) {
                throw errorLibrary("Can't read instrument data");
            }

            chunk = new Instrument;
            chunk->fromRaw(buffer);
        }
        break;

    case ChunkLoopInfo:
        SF_LOOP_INFO li;
        size = sizeof li;

        if (sf_command(_handle, SFC_GET_LOOP_INFO, &li, size)) {
            buffer.resize(size);
            memcpy(buffer.data(), &li, size);
            chunk = new LoopInfo;
            chunk->fromRaw(buffer);
        }
        break;

    default:
        throw errorFile("Requested unimplemented chunk type for reading");
    }

    _chunks[type] = chunk;
}

bool Input::read(short *ptr, int frames)
{
    return sf_readf_short(_handle, ptr, frames) == frames;
}

bool Input::read(int *ptr, int frames)
{
    return sf_readf_int(_handle, ptr, frames) == frames;
}

bool Input::read(float *ptr, int frames)
{
    return sf_readf_float(_handle, ptr, frames) == frames;
}

bool Input::read(double *ptr, int frames)
{
    return sf_readf_double(_handle, ptr, frames) == frames;
}

const Chunk *Input::chunk(ChunkType type)
{
    bool isTested = _chunksTested & (1 << type);

    if (!isTested) {
        loadChunk(type);
        _chunksTested |= (1 << type);
    }

    return _chunks[type];
}

Input::RawChunkList Input::rawChunks(const QString &id)
{
    /*SF_CHUNK_INFO info;
    memset(&info, 0, sizeof info);

    if (!id.isEmpty()) {
        QByteArray bid = id.toUtf8();
        bid.truncate(sizeof info.id);
        memcpy(info.id, bid.data(), bid.length());
        info.id_size = bid.length();
    }

    RawChunkList result;
    SF_CHUNK_ITERATOR *iterator =
            sf_get_chunk_iterator(_handle, &info);

    while (iterator) {
        if (sf_get_chunk_data(iterator, &info) != SF_ERR_NO_ERROR) {
            throw errorLibrary(QString("Can't read raw chunk with id \"%1\"").arg(id));
        }

        RawChunk chunk;
        chunk.id.fromLocal8Bit(info.id, info.id_size);
        chunk.data.resize(info.datalen);
        memcpy(chunk.data.data(), info.data, info.datalen);
        result.append(chunk);
    }

    return result; */
}

QString Input::string(StringEntry entry)
{
    const char *result = sf_get_string(_handle, entry);

    if (NULL == result) {
        throw errorLibrary(QString("Unable to get string entry %1").arg(entry));
    }

    return QString::fromUtf8(result);
}
