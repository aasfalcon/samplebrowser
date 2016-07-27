#ifndef FILE_H
#define FILE_H

#include <QDateTime>

#include "format.h"

namespace Sound {

class FileError;
class LibraryError;

class File
{
public:
    enum ChunkType {
        ChunkBroadcastInfo,
        ChunkLoopInfo,
        ChunkInstrument,
        ChunkCartInfo,
        ChunkCues,

        ChunkTypeCount
    };

    enum StringEntry {
        StringTitle = 1,
        StringCopyrhight,
        StringSoftware,
        StringArtist,
        StringComment,
        StringDate,
        StringAlbum,
        StringLicense,
        StringTrackNumber,
        StringGenre,

        StringEntryCount
    };

    ~File();
    int channels() const;
    Format format() const;
    int frames() const;
    QString path() const;
    int sampleRate() const;

protected:
    int _channels;
    struct SNDFILE_tag *_handle;
    Format _format;
    int _frames;
    QString _path;
    int _sampleRate;

    File();
    File(const QString &path, int channels, Format format, int frames, int sampleRate);
    FileError errorFile(const QString &message) const;
    LibraryError errorLibrary(const QString &message) const;
};

} // namespace Sound

#endif // FILE_H
