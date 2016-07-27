#ifndef INPUT_H
#define INPUT_H

#include "error.h"
#include "file.h"
#include "buffer.h"
#include "chunk.h"

namespace Sound {

class Input : public File {
public:
    struct RawChunk {
        QString id;
        QByteArray data;
    };

    typedef QList<RawChunk> RawChunkList;

    Input(const QString &path);
    ~Input();

    const Chunk *chunk(ChunkType type);
    RawChunkList rawChunks(const QString &id = "");

    template<class T>
    void read(Buffer<T> &buffer, int frames) {
        buffer = Buffer<T>(frames, channels());

        if (!read(buffer.data(), frames)) {
            throw errorLibrary("Unable to read file data");
        }
    }

    QString string(StringEntry entry);
    bool wavexAmbisonic();

private:
    Chunk *_chunks[ChunkTypeCount];
    int _chunksTested;
    void loadChunk(ChunkType type);

    bool read(short *ptr, int frames);
    bool read(int *ptr, int frames);
    bool read(float *ptr, int frames);
    bool read(double *ptr, int frames);
};

} // namespace Sound

#endif // INPUT_H
