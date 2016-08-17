#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include "buffer.h"
#include "basicstream.h"

class InputStream: virtual public BasicInputStream
{
public:
    inline InputStream(const std::string &path);

    inline unsigned pos() const;
    inline void seek(int pos, IAudioFile::SeekWhence whence);

    inline InputStream &operator >>(Compression &compression);
    inline InputStream &operator >>(Chunk &chunk);
    inline InputStream &operator >>(Format &format);
    inline InputStream &operator >>(FormatText &formatText);
    inline InputStream &operator >>(Info &info);
    inline InputStream &operator >>(Strings &strings);
    inline InputStream &operator >>(RawChunks &rawChunks);

    template<typename T>
    InputStream &operator >>(Buffer<T> &buffer);

protected:
    inline InputStream();
};

#include "inputstream.tcc"

#endif // INPUTSTREAM_H
