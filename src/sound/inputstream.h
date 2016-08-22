#ifndef SOUND_INPUTSTREAM_H
#define SOUND_INPUTSTREAM_H

#include "basicstream.h"
#include "buffer.h"

namespace Sound {

class InputStream : virtual public BasicInputStream {
public:
    InputStream(const std::string& path)
    {
        open(path, IAudioFile::ModeRead);
    }

    unsigned pos() const
    {
        return BasicStream::pos(IAudioFile::SeekTypeRead);
    }
    void seek(int pos, IAudioFile::SeekWhence whence)
    {
        BasicStream::seek(pos, whence, IAudioFile::SeekTypeRead);
    }

    InputStream& operator>>(Compression& compression)
    {
        read(compression);
        return *this;
    }

    InputStream& operator>>(Chunk& chunk)
    {
        read(chunk);
        return *this;
    }

    InputStream& operator>>(Format& format)
    {
        read(format);
        return *this;
    }

    InputStream& operator>>(FormatText& formatText)
    {
        read(formatText);
        return *this;
    }

    InputStream& operator>>(Info& info)
    {
        read(info);
        return *this;
    }

    InputStream& operator>>(Strings& strings)
    {
        read(strings);
        return *this;
    }

    InputStream& operator>>(RawChunks& rawChunks)
    {
        read(rawChunks);
        return *this;
    }

    template <typename T>
    InputStream& operator>>(Buffer<T>& buffer)
    {
        read(buffer.data(), buffer.type(), buffer.frames());
        return *this;
    }

protected:
    InputStream() {}
};
}

#endif // SOUND_INPUTSTREAM_H
