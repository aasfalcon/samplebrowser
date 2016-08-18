#ifndef AUDIOFILESNDFILE_H
#define AUDIOFILESNDFILE_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "includes/libsndfile/sndfile.h"
#include "shared/iaudiofile.h"

class AudioFileSndfile: public IAudioFile
{
public:
    AudioFileSndfile();
    ~AudioFileSndfile();

    const ChunkData *chunk(ChunkType type, unsigned *size);
    void close();
    void flush();
    FormatText &formatText(const Format &format);
    const SimpleFormats &formats();
    const FileInfo *fileInfo() const;
    void open(const char *filename, Mode mode);
    RawChunk *rawChunks(unsigned *count);
    unsigned read(void *buffer, Sound::Type type, unsigned frames);
    unsigned seek(int pos, SeekWhence sw, SeekType st = SeekTypeDefault);
    void setChunk(ChunkType type, unsigned size, const ChunkData *data);
    void setFileInfo(const FileInfo *value);
    void setRawChunks(RawChunk *chunks, unsigned count);
    void setString(StringEntry entry, const char *value);
    void setProgressCallback(ProgressCallback callback);
    const char *string(StringEntry entry);
    void write(const void *buffer, unsigned frames);

protected:
    FileInfo _info;
    std::string _path;

private:
    std::map<ChunkType, std::vector<ChunkData>> _chunks;
    bool _chunksFlushed;
    bool _flushed;
    std::shared_ptr<FormatText> _formatText;
    static std::map<int, FormatMajor> _majorMap;
    static std::map<int, FormatMinor> _minorMap;
    std::shared_ptr<SNDFILE> _handle;
    Mode _mode;
    ProgressCallback _progress;
    std::vector<RawChunk> _rawChunks;

    struct InternalRawChunk {
        std::string id;
        std::vector<ChunkData> data;
    };

    std::vector<InternalRawChunk> _rawChunksData;
    std::vector<SimpleFormats::Formats> _simpleFormatsData;
    std::shared_ptr<SimpleFormats> _simpleFormats;

    Format formatConvert(int sfformat) const;
    int formatConvert(const Format &format) const;
    void flushChunks();
    std::string sndfileError(int errorNumber, const std::string &userMessage) const;
};

#endif // AUDIOFILESNDFILE_H
