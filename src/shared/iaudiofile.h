#ifndef IAUDIOFILE_H
#define IAUDIOFILE_H

#include "interface.h"
#include "../sound/sound.h"
#include <sndfile.h>

struct IAudioFile: Interface {
    typedef void (*ProgressCallback)(unsigned percent);
    typedef unsigned char ChunkData;

    enum ChunkType {
        ChunkBroadcastInfo,
        ChunkLoopInfo,
        ChunkInstrument,
        ChunkCartInfo,
        ChunkCues,

        ChunkTypeCount
    };

    enum FormatMajor {
        MajorWAV,
        MajorAIFF,
        MajorAU,
        MajorRAW,
        MajorPAF,
        MajorSVX,
        MajorNIST,
        MajorVOC,
        MajorIRCAM,
        MajorW64,
        MajorMAT4,
        MajorMAT5,
        MajorPVF,
        MajorXI,
        MajorHTK,
        MajorSDS,
        MajorAVR,
        MajorWAVEX,
        MajorSD2,
        MajorFLAC,
        MajorCAF,
        MajorWVE,
        MajorOGG,
        MajorMPC2K,
        MajorRF64,

        MajorMP1,
        MajorMP2,
        MajorMP3
    };

    enum FormatMinor {
        MinorPCM_S8,
        MinorPCM_16,
        MinorPCM_24,
        MinorPCM_32,
        MinorPCM_U8,
        MinorFloat,
        MinorDouble,
        MinorULAW,
        MinorALAW,
        MinorIMA_ADPCM,
        MinorMS_ADPCM,
        MinorGSM610,
        MinorVOX_ADPCM,
        MinorG721_32k,
        MinorG723_24k,
        MinorG723_40k,
        MinorDWVW_12,
        MinorDWVW_16,
        MinorDWVW_24,
        MinorDWVW_N,
        MinorDPCM_8,
        MinorDPCM_16,
        MinorVorbis,
        MinorALAC_16,
        MinorALAC_20,
        MinorALAC_24,
        MinorALAC_32,

        MinorMPEG1_0_Mono,
        MinorMPEG1_0_Stereo,
        MinorMPEG1_0_JointStereo,
        MinorMPEG1_0_DualChannel,
        MinorMPEG2_0_Mono,
        MinorMPEG2_0_Stereo,
        MinorMPEG2_0_JointStereo,
        MinorMPEG2_0_DualChannel,
        MinorMPEG2_5_Mono,
        MinorMPEG2_5_Stereo,
        MinorMPEG2_5_JointStereo,
        MinorMPEG2_5_DualChannel
    };

    enum Mode {
        ModeClosed,
        ModeRead,
        ModeWrite,
        ModeReadWrite,

        ModeCount
    };

    enum StringEntry {
        StringTitle,
        StringCopyrhight,
        StringSoftware,
        StringArtist,
        StringComment,
        StringDate,
        StringAlbum,
        StringLicense,
        StringTrackNumber,
        StringGenre,

        StringEntryCount
    };

    enum WavexAmbisonic {
        AmbisonicNotSupported,
        AmbisonicNone,
        AmbisonicBFormat,
    };

    enum SeekWhence {
        SeekCur,
        SeekEnd,
        SeekSet,
    };

    enum SeekType {
        SeekTypeReadAndWrite,
        SeekTypeDefault = SeekTypeReadAndWrite,
        SeekTypeRead,
        SeekTypeWrite,
    };

    enum CompressionType {
        CompressionABR,
        CompressionCBR,
        CompressionVBR,
    };

    struct CompressionInfo {
        union {
            WavexAmbisonic ambisonic;
            CompressionType type;
        };

        unsigned bitrate;
        double level;
        double quality;
    };

    struct Format {
        FormatMajor major;
        FormatMinor minor;
    };

    struct FormatText {
        const char *majorText;
        const char *minorText;
    };

    struct SimpleFormats {
        unsigned count;

        struct Formats {
            const char *description;
            const char *extension;
            Format format;
        } *formats;
    };

    struct FileInfo {
        unsigned channels;
        unsigned chunkFlags; // | (1 << ChunkType)
        CompressionInfo compression;
        Format format;
        unsigned frames;
        Sound::Type sampleType;
        unsigned sampleRate;
        bool seekable;
    };

    struct RawChunk {
        const char *id;
        const ChunkData *data;
        unsigned size;
    };

    virtual const ChunkData *chunk(ChunkType type, unsigned *size) = 0;
    virtual void close() = 0;
    virtual const FileInfo *fileInfo() const = 0;
    virtual void flush() = 0;
    virtual FormatText &formatText(const Format &format) = 0;
    virtual const SimpleFormats &formats() = 0;
    virtual void open(const char *filename, Mode mode) = 0;
    virtual RawChunk *rawChunks(unsigned *count) = 0;
    virtual unsigned read(void *buffer, Sound::Type type, unsigned frames) = 0;
    virtual unsigned seek(int pos, SeekWhence sw, SeekType st = SeekTypeDefault) = 0;
    virtual void setChunk(ChunkType type, unsigned size, const ChunkData *data) = 0;
    virtual void setFileInfo(const FileInfo *value) = 0;
    virtual void setProgressCallback(ProgressCallback callback) = 0;
    virtual void setRawChunks(RawChunk *chunks, unsigned count) = 0;
    virtual void setString(StringEntry entry, const char *value) = 0;
    virtual const char *string(StringEntry entry) = 0;
    virtual void write(const void *buffer, unsigned frames) = 0;
};

#endif // IAUDIOFILE_H
