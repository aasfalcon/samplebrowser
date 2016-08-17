#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include "buffer.h"
#include "basicstream.h"

class OutputStream: virtual public BasicOutputStream
{
public:
    inline OutputStream(const std::string &path);

    inline unsigned pos() const;
    inline void seek(int pos, IAudioFile::SeekWhence whence);

    inline OutputStream &operator <<(const Compression &compression);
    inline OutputStream &operator <<(const Chunk &chunk);
    inline OutputStream &operator <<(const Format &format);
    inline OutputStream &operator <<(const Info &info);
    inline OutputStream &operator <<(const Strings &strings);
    inline OutputStream &operator <<(const RawChunk &rawChunk);
    inline OutputStream &operator <<(const RawChunks &rawChunks);

    template<typename T>
    OutputStream &operator <<(const Buffer<T> &buffer);
};

#include "outputstream.tcc"

#endif // OUTPUTSTREAM_H
