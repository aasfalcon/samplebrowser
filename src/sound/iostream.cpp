#include "iostream.h"

IOStream::IOStream(const std::string& path)
{
    open(path, IAudioFile::ModeReadWrite);
}

unsigned IOStream::pos() const
{
    return BasicStream::pos(IAudioFile::SeekTypeReadAndWrite);
}

void IOStream::seek(int pos, IAudioFile::SeekWhence whence)
{
    BasicStream::seek(pos, whence, IAudioFile::SeekTypeReadAndWrite);
}

IOStream& IOStream::operator>>(Compression& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(Chunk& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(Format& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(FormatText& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(Info& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(Strings& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator>>(RawChunks& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

template <typename T>
IOStream& IOStream::operator>>(Buffer<T>& rht)
{
    return dynamic_cast<IOStream&>(InputStream::operator>>(rht));
}

IOStream& IOStream::operator<<(const Compression& compression)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(compression));
}

IOStream& IOStream::operator<<(const Chunk& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

IOStream& IOStream::operator<<(const Format& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

IOStream& IOStream::operator<<(const Info& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

IOStream& IOStream::operator<<(const Strings& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

IOStream& IOStream::operator<<(const RawChunk& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

IOStream& IOStream::operator<<(const RawChunks& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}

template <typename T>
IOStream& IOStream::operator<<(const Buffer<T>& rht)
{
    return dynamic_cast<IOStream&>(OutputStream::operator<<(rht));
}
