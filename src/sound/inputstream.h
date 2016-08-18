#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include "basicstream.h"
#include "buffer.h"

class InputStream : virtual public BasicInputStream {
public:
    InputStream(const std::string& path);

    unsigned pos() const;
    void seek(int pos, IAudioFile::SeekWhence whence);

    InputStream& operator>>(Compression& compression);
    InputStream& operator>>(Chunk& chunk);
    InputStream& operator>>(Format& format);
    InputStream& operator>>(FormatText& formatText);
    InputStream& operator>>(Info& info);
    InputStream& operator>>(Strings& strings);
    InputStream& operator>>(RawChunks& rawChunks);

    template <typename T>
    InputStream& operator>>(Buffer<T>& buffer);

protected:
    InputStream();
};

#endif // INPUTSTREAM_H
