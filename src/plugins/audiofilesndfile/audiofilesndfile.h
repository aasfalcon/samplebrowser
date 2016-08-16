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

    void close();
    void flush();
    void open(const char *filename, Mode mode);

    const ChunkData *chunk(const char *id, unsigned *size);
    const ChunkData *chunk(ChunkType type, unsigned *size);
    void setChunk(const char *id, unsigned size, const ChunkData *data);
    void setChunk(ChunkType type, unsigned size, const ChunkData *data);

    const FileInfo *fileInfo() const;
    void setFileInfo(const FileInfo *value);

    void setProgressCallback(ProgressCallback callback);

    unsigned read(void *buffer, unsigned frames);
    unsigned seek(int offset, SeekWhence sw, SeekType st = SeekTypeDefault);
    void write(const void *buffer, unsigned frames);

protected:
    FileInfo _info;
    std::string _path;

private:
    std::map<ChunkType, std::vector<ChunkData>> _chunks;
    bool _chunksFlushed;
    std::map<std::string, std::vector<ChunkData>> _chunksRaw;
    bool _flushed;
    std::shared_ptr<SNDFILE> _handle;
    Mode _mode;
    ProgressCallback _progress;

    std::string sndfileError(int errorNumber, const std::string &userMessage);
    void flushChunks();
};

#endif // AUDIOFILESNDFILE_H
