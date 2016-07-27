#include "libsndfile/src/sndfile.h"

#include "error.h"
#include "file.h"
#include "chunk.h"

using namespace Sound;

File::File()
    : _handle(NULL)
{
}

File::File(const QString &path, int channels, Format format, int frames, int sampleRate)
    : _channels(channels)
    , _handle(NULL)
    , _format((int)format)
    , _frames(frames)
    , _path(path)
    , _sampleRate(sampleRate)
{
}

File::~File()
{
    if (_handle) {
        sf_close(_handle);
    }
}

FileError File::errorFile(const QString &message) const
{
    FileError result;
    result.message = message;
    result.path = _path;
    return result;
}

LibraryError File::errorLibrary(const QString &message) const
{
    LibraryError result;
    result.error = sf_strerror(_handle);
    result.path = _path;
    result.message = message;
    result.version = sf_version_string();

    QByteArray logInfo(2048, 0);
    sf_command(_handle, SFC_GET_LOG_INFO, logInfo.data(), logInfo.size());
    result.log.fromUtf8(logInfo);

    return result;
}

Format File::format() const
{
    return _format;
}

int File::frames() const
{
    return _frames;
}

int File::channels() const
{
    return _channels;
}

QString File::path() const
{
    return _path;
}

int File::sampleRate() const
{
    return _sampleRate;
}
