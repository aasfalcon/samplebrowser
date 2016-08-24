#ifndef SOUND_STREAM_OUTPUTSTREAM_H
#define SOUND_STREAM_OUTPUTSTREAM_H

#include "basicstream.h"
#include "buffer.h"

namespace Sound {

class OutputStream : virtual public BasicOutputStream {
public:
    OutputStream(const std::string& path)
    {
        open(path, IAudioFile::ModeWrite);
    }

    unsigned pos() const
    {
        return BasicStream::pos(IAudioFile::SeekTypeWrite);
    }

    void seek(int pos, IAudioFile::SeekWhence whence)
    {
        BasicStream::seek(pos, whence, IAudioFile::SeekTypeWrite);
    }

    OutputStream& operator<<(const Compression& compression)
    {
        write(compression);
        return *this;
    }

    OutputStream& operator<<(const Chunk& chunk)
    {
        write(chunk);
        return *this;
    }

    OutputStream& operator<<(const Format& format)
    {
        write(format);
        return *this;
    }

    OutputStream& operator<<(const Info& info)
    {
        write(info);
        return *this;
    }

    OutputStream& operator<<(const Strings& strings)
    {
        write(strings);
        return *this;
    }

    OutputStream& operator<<(const RawChunk& rawChunk)
    {
        write(rawChunk);
        return *this;
    }

    OutputStream& operator<<(const RawChunks& rawChunks)
    {
        write(rawChunks);
        return *this;
    }

    template <typename T>
    OutputStream& operator<<(const Buffer<T>& buffer)
    {
        _provider->write(buffer.data(), buffer.frames());
        return *this;
    }

protected:
    OutputStream() {}
};
}

#endif // SOUND_STREAM_OUTPUTSTREAM_H
