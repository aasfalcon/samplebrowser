#include "libsndfile/src/sndfile.h"

#include "output.h"
#include "input.h"
#include "error.h"

using namespace Sound;

void Output::chunkProxy(Input &inputFile)
{
    Input::RawChunkList chunks = inputFile.rawChunks();

    for (int i = 0; i < chunks.count(); i++) {
        const Input::RawChunk &chunk = chunks[i];
        setRawChunk(chunk.id, chunk.data);
    }
}

void Output::setChunk(ChunkType type, const Chunk &chunk)
{
    QByteArray raw = chunk.toRaw();

    switch (type) {
    case ChunkBroadcastInfo:
        if (!sf_command(_handle, SFC_SET_BROADCAST_INFO, raw.data(), raw.count())) {
            throw errorLibrary("Unable to set broadcast info chunk");
        }

        break;

    case ChunkCartInfo:
        if (!sf_command(_handle, SFC_SET_CART_INFO, raw.data(), raw.count())) {
            throw errorLibrary("Unable to set cart info chunk");
        }

        break;

    case ChunkCues:
        if (!sf_command(_handle, SFC_SET_CUE, raw.data(), raw.count())) {
            throw errorLibrary("Unable to set cues points chunk");
        }

        break;

    case ChunkInstrument:
        if (!sf_command(_handle, SFC_SET_INSTRUMENT, raw.data(), raw.count())) {
            throw errorLibrary("Unable to set instrument chunk");
        }

        break;

    case ChunkLoopInfo: {
        const LoopInfo &loop = (const LoopInfo &) chunk;
        int flags = 0;

        if (loop.mode == LoopInfo::Forward) flags |= 0x01;
        if (!loop.rootKey) flags |= 0x02;

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
            loop.rootKey,
            0x8000,
            .0,
            loop.beats,
            loop.timeSigDen,
            loop.timeSigNum,
            loop.bpm,
        };

        setRawChunk("acid", QByteArray((const char *)&acid, sizeof acid));
        break;
    }

    default:
        throw errorFile("Attempt to write unimplemented chunk type");
    }
}

void Output::setCompressionLevel(double value)
{
    if (SF_FALSE == sf_command(_handle, SFC_SET_VBR_ENCODING_QUALITY, &value, sizeof value)) {
        throw errorLibrary("Error setting output VBR quality");
    }
}

void Output::setRawChunk(const QString &id, const QByteArray &source)
{
    SF_CHUNK_INFO info;
    QByteArray bid = id.toUtf8();
    bid.truncate(sizeof info.id);
    memcpy(info.id, bid.data(), bid.length());
    info.id_size = bid.length();

    QByteArray buffer(source);
    info.data = buffer.data();
    info.datalen = buffer.length();

    if (sf_set_chunk(_handle, &info) != SF_ERR_NO_ERROR) {
        throw errorLibrary(QString("Can't write raw chunk with id \"%1\"").arg(id));
    }
}

void Output::setString(StringEntry entry, const QString &value)
{
    if (sf_set_string(_handle, entry, value.toUtf8())) {
        throw errorLibrary(QString("Error setting string entry %1, value \"%2\"")
                           .arg(entry).arg(value));
    }
}

void Output::setVbrEncodingQuality(double value)
{
    if (SF_FALSE == sf_command(_handle, SFC_SET_VBR_ENCODING_QUALITY, &value, sizeof value)) {
        throw errorLibrary("Error setting output VBR quality");
    }
}

void Output::setWavexAmbisonic(bool value)
{
    int sf_value = value ? SF_AMBISONIC_B_FORMAT : SF_AMBISONIC_NONE;

    if (!sf_command(_handle, SFC_WAVEX_GET_AMBISONIC, NULL, sf_value)) {
        throw errorLibrary("Error setting WAVEX ambisonic");
    }
}

void Output::truncate(int frames)
{
    if (sf_command(_handle, SFC_FILE_TRUNCATE, &frames, sizeof frames)) {
        throw errorLibrary(QString("Error truncating file to %1 frames").arg(frames));
    }
}

bool Output::write(short *ptr, int frames)
{
    return sf_writef_short(_handle, ptr, frames) == frames;
}

bool Output::write(int *ptr, int frames)
{
    return sf_writef_int(_handle, ptr, frames) == frames;
}

bool Output::write(float *ptr, int frames)
{
    return sf_writef_float(_handle, ptr, frames) == frames;
}

bool Output::write(double *ptr, int frames)
{
    return sf_writef_double(_handle, ptr, frames) == frames;
}

