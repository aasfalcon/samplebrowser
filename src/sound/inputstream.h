#ifndef INPUTSTREAM_H
#define INPUTSTREAM_H

#include <map>
#include <memory>
#include <string>
#include <thread>

#include "buffer.h"
#include "chunk.h"
#include "sound.h"
#include "shared/iaudiofile.h"

struct InputStreamInfo {
    unsigned channels;
    std::string formatMajor;
    std::string formatMinor;
    unsigned frames;
    Sound::Type sampleType;
    unsigned sampleRate;
};

typedef std::map<IAudioFile::StringEntry, std::string> InputStreamStrings;

template<typename T>
class InputStream: public Object<T>
{
public:
    InputStream(const std::string &path, bool readAheadNow = false);

    bool eof() const;
    unsigned bufferSize() const;
    bool isSeekable() const;
    std::string path() const;
    unsigned pos() const;
    unsigned seek(int pos, IAudioFile::SeekWhence whence);
    void setBufferSize(unsigned value);

    InputStream &operator >>(Chunk &chunk);
    InputStream &operator >>(InputStreamInfo &info);
    InputStream &operator >>(InputStreamStrings &strings);
    InputStream &operator >>(Buffer<T> &buffer);

private:
    const unsigned DEFAULT_BUFFER_SIZE = 0xFFFF;
    unsigned _bufferSize;
    Buffer<T> _current;
    bool _isPreloaded;
    bool _isSwapped;
    Buffer<T> _next;
    std::string _path;
    std::thread _preloadThread;
    std::shared_ptr<IAudioFile> _provider;

    void drop();
    void preload();
    void readAhead();
    void swap();
};

SOUND_INSTANTIATE_DECLARATION(InputStream)

#endif // INPUTSTREAM_H
