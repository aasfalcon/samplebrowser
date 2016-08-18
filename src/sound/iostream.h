#ifndef IOStream_H
#define IOStream_H

#include "inputstream.h"
#include "outputstream.h"

class IOStream
    : virtual public InputStream,
      virtual public OutputStream {
public:
    IOStream(const std::string& path);

    unsigned pos() const;
    void seek(int pos, IAudioFile::SeekWhence whence);

    IOStream& operator>>(Compression& rht);
    IOStream& operator>>(Chunk& rht);
    IOStream& operator>>(Format& rht);
    IOStream& operator>>(FormatText& rht);
    IOStream& operator>>(Info& rht);
    IOStream& operator>>(Strings& rht);
    IOStream& operator>>(RawChunks& rht);

    template <typename T>
    IOStream& operator>>(Buffer<T>& rht);

    IOStream& operator<<(const Compression& rht);
    IOStream& operator<<(const Chunk& rht);
    IOStream& operator<<(const Format& rht);
    IOStream& operator<<(const Info& rht);
    IOStream& operator<<(const Strings& rht);
    IOStream& operator<<(const RawChunk& rht);
    IOStream& operator<<(const RawChunks& rht);

    template <typename T>
    IOStream& operator<<(const Buffer<T>& rht);
};

#endif // IOStream_H
