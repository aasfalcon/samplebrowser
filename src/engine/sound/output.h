#ifndef OUTPUT_H
#define OUTPUT_H

#include "buffer.h"
#include "error.h"
#include "file.h"

namespace Sound {

struct Chunk;
class Input;

class Output : public File {
public:
    Output(const QString &path, Format format, int channels, int sampleRate);
    ~Output();

    void chunkProxy(Input &inputFile);
    void setChunk(ChunkType type, const Chunk &chunk);
    void setCompressionLevel(double value);
    void setRawChunk(const QString &id, const QByteArray &source);
    void setString(StringEntry entry, const QString &value);
    void setVbrEncodingQuality(double value);
    void setWavexAmbisonic(bool value);

    void truncate(int frames);

    template<class T>
    void write(const Buffer<T> &buffer) {
        if (!write(buffer.data(), buffer.frames())) {
            throw errorLibrary("Unable to write sound data");
        }
    }

private:
    bool write(short *ptr, int frames);
    bool write(int *ptr, int frames);
    bool write(float *ptr, int frames);
    bool write(double *ptr, int frames);
};

} // namespace Sound

#endif // OUTPUT_H
