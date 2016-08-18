#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include "basicstream.h"
#include "buffer.h"

class OutputStream : virtual public BasicOutputStream {
public:
    OutputStream(const std::string& path);

    unsigned pos() const;
    void seek(int pos, IAudioFile::SeekWhence whence);

    OutputStream& operator<<(const Compression& compression);
    OutputStream& operator<<(const Chunk& chunk);
    OutputStream& operator<<(const Format& format);
    OutputStream& operator<<(const Info& info);
    OutputStream& operator<<(const Strings& strings);
    OutputStream& operator<<(const RawChunk& rawChunk);
    OutputStream& operator<<(const RawChunks& rawChunks);

    template <typename T>
    OutputStream& operator<<(const Buffer<T>& buffer);

protected:
    OutputStream();
};

#endif // OUTPUTSTREAM_H
