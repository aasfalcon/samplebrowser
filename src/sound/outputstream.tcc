#include "shared/log.h"
#include "outputstream.h"

OutputStream::OutputStream(const std::string &path)
{
    open(path, IAudioFile::ModeWrite);
}

unsigned OutputStream::pos() const
{
    return BasicStream::pos(IAudioFile::SeekTypeWrite);
}

void OutputStream::seek(int pos, IAudioFile::SeekWhence whence)
{
    BasicStream::seek(pos, whence, IAudioFile::SeekTypeWrite);
}

OutputStream &OutputStream::operator <<(const Compression &compression)
{
    write(compression);
    return *this;
}

OutputStream &OutputStream::operator <<(const Chunk &chunk)
{
    write(chunk);
    return *this;
}

OutputStream &OutputStream::operator <<(const Format &format)
{
    write(format);
    return *this;
}

OutputStream &OutputStream::operator <<(const Info &info)
{
    write(info);
    return *this;
}

OutputStream &OutputStream::operator <<(const Strings &strings)
{
    write(strings);
    return *this;
}

OutputStream &OutputStream::operator <<(const RawChunk &rawChunk)
{
    write(rawChunk);
    return *this;
}

OutputStream &OutputStream::operator <<(const RawChunks &rawChunks)
{
    write(rawChunks);
    return *this;
}

template<typename T>
OutputStream &OutputStream::operator <<(const Buffer<T> &buffer)
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

    _provider->write(buffer.data(), buffer.type(), frames);
    return *this;
}
