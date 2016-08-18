#include <stdexcept>

#include "inputstream.h"
#include "shared/log.h"

InputStream::InputStream()
{
}

InputStream::InputStream(const std::string& path)
{
    open(path, IAudioFile::ModeRead);
}

unsigned InputStream::pos() const
{
    return BasicStream::pos(IAudioFile::SeekTypeRead);
}

void InputStream::seek(int pos, IAudioFile::SeekWhence whence)
{
    BasicStream::seek(pos, whence, IAudioFile::SeekTypeRead);
}

InputStream& InputStream::operator>>(Compression& compression)
{
    read(compression);
    return *this;
}

InputStream& InputStream::operator>>(Chunk& chunk)
{
    read(chunk);
    return *this;
}

InputStream& InputStream::operator>>(Format& format)
{
    read(format);
    return *this;
}

InputStream& InputStream::operator>>(FormatText& formatText)
{
    read(formatText);
    return *this;
}

InputStream& InputStream::operator>>(Info& info)
{
    read(info);
    return *this;
}

InputStream& InputStream::operator>>(Strings& strings)
{
    read(strings);
    return *this;
}

InputStream& InputStream::operator>>(RawChunks& rawChunks)
{
    read(rawChunks);
    return *this;
}

template <typename T>
InputStream& InputStream::operator>>(Buffer<T>& buffer)
{
    unsigned frames = buffer.frames();

    if (!frames) {
        std::string message = "Zero buffer size";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    if (buffer.channels() != this->_fi.channels) {
        std::string message = "Buffer channel count doesn't match";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    read(buffer.data(), buffer.type(), frames);
    return *this;
}

#define READ_BUFFER(__type) \
    template InputStream& InputStream::operator>><__type>(Buffer<__type>&);

SOUND_INSTANTIATE_METHOD(READ_BUFFER);
