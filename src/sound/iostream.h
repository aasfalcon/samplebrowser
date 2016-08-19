#ifndef IOStream_H
#define IOStream_H

#include "inputstream.h"
#include "outputstream.h"

namespace Sound {

class IOStream
    : virtual public InputStream,
      virtual public OutputStream {
public:
    IOStream(const std::string& path)
    {
        open(path, IAudioFile::ModeReadWrite);
    }

    unsigned pos() const
    {
        return BasicStream::pos(IAudioFile::SeekTypeReadAndWrite);
    }

    void seek(int pos, IAudioFile::SeekWhence whence)
    {
        BasicStream::seek(pos, whence, IAudioFile::SeekTypeReadAndWrite);
    }

    IOStream& operator>>(Compression& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(Chunk& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(Format& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(FormatText& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(Info& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(Strings& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator>>(RawChunks& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    template <typename T>
    IOStream& operator>>(Buffer<T>& rht)
    {
        return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
    }

    IOStream& operator<<(const Compression& compression)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(compression));
    }

    IOStream& operator<<(const Chunk& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    IOStream& operator<<(const Format& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    IOStream& operator<<(const Info& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    IOStream& operator<<(const Strings& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    IOStream& operator<<(const RawChunk& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    IOStream& operator<<(const RawChunks& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }

    template <typename T>
    IOStream& operator<<(const Buffer<T>& rht)
    {
        return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
    }
};
}

#endif // IOStream_H
