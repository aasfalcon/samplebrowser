#ifndef IOStream_H
#define IOStream_H

#include "inputstream.h"
#include "outputstream.h"

class IOStream
        : virtual public InputStream
        , virtual public OutputStream
{
public:
    inline IOStream(const std::string &path);

    inline unsigned pos() const;
    inline void seek(int pos, IAudioFile::SeekWhence whence);

    inline IOStream &operator >>(Compression &rht);
    inline IOStream &operator >>(Chunk &rht);
    inline IOStream &operator >>(Format &rht);
    inline IOStream &operator >>(FormatText &rht);
    inline IOStream &operator >>(Info &rht);
    inline IOStream &operator >>(Strings &rht);
    inline IOStream &operator >>(RawChunks &rht);

    template<typename T>
    inline IOStream &operator >>(Buffer<T> &rht);

    inline IOStream &operator <<(const Compression &rht);
    inline IOStream &operator <<(const Chunk &rht);
    inline IOStream &operator <<(const Format &rht);
    inline IOStream &operator <<(const Info &rht);
    inline IOStream &operator <<(const Strings &rht);
    inline IOStream &operator <<(const RawChunk &rht);
    inline IOStream &operator <<(const RawChunks &rht);

    template<typename T>
    inline IOStream &operator <<(const Buffer<T> &rht);
};

#include "iostream.tcc"

#endif // IOStream_H
