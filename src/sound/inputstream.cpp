#include "sample.h"
#include "inputstream.h"
#include "shared/server.h"

template<typename T>
InputStream<T>::InputStream(const std::string &path, bool readAheadNow)
    : Object<T> ()
    , _bufferSize(DEFAULT_BUFFER_SIZE)
    , _isPreloaded(false)
    , _path(path)
    , _provider(PLUGIN_FACTORY(IAudioFile))
{
    _provider->open(path.c_str(), IAudioFile::ModeRead);

    if (readAheadNow) {
        readAhead();
    }
}

template<typename T>
unsigned InputStream<T>::bufferSize() const
{
    return _bufferSize;
}

template<typename T>
void InputStream<T>::drop()
{
    if (_isPreloaded) {
        _provider->seek(- int(_current.frames()) - int(_next.frames()),
                        IAudioFile::SeekCur);
        _isPreloaded = false;
    }
}

template<typename T>
bool InputStream<T>::eof() const
{
    if (!_provider) {
        return true;
    }

    const IAudioFile::FileInfo &info = *_provider->fileInfo();
    return _provider->seek(0, IAudioFile::SeekCur) == info.frames
            && (!_isPreloaded || _current.isEmpty());
}

template<typename T>
bool InputStream<T>::isSeekable() const
{
    const IAudioFile::FileInfo &info = *_provider->fileInfo();
    return info.seekable;
}

template<typename T>
std::string InputStream<T>::path() const
{
    return _path;
}

template<typename T>
unsigned InputStream<T>::pos() const
{
    unsigned result = _provider->seek(0, IAudioFile::SeekCur);

    if (_isPreloaded) {
        result -= _current.frames() + _next.frames();
    }

    return result;
}

template<typename T>
unsigned InputStream<T>::seek(int pos, IAudioFile::SeekWhence whence)
{
    drop();
    unsigned result = _provider->seek(pos, whence);
    readAhead();
    return result;
}

template<typename T>
void InputStream<T>::setBufferSize(unsigned value)
{
    if (_bufferSize != value) {
        _bufferSize = value;

        if (_isPreloaded) {
            drop();
            readAhead();
        }
    }
}

template<typename T>
InputStream<T> &InputStream<T>::operator >>(Chunk &chunk)
{
    const IAudioFile::ChunkData *ptr = nullptr;
    unsigned size = 0;

    if (dynamic_cast<BroadcastInfo *>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkBroadcastInfo, &size);
    } else if (dynamic_cast<LoopInfo *>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkLoopInfo, &size);
    } else if (dynamic_cast<Instrument *>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkInstrument, &size);
    } else if (dynamic_cast<CartInfo *>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkCartInfo, &size);
    } else if (dynamic_cast<Cues *>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkCues, &size);
    }

    if (ptr && size) {
        Chunk::RawData data;
        data.assign(ptr, ptr + size);
        chunk.fromRaw(data);
    }

    return *this;
}

template<typename T>
void InputStream<T>::preload()
{
    if (_isPreloaded) {
        throw std::runtime_error("Stream buffer already preloaded");
    }

    const IAudioFile::FileInfo &info = *_provider->fileInfo();
    _next.reallocate(info.channels, _bufferSize);
    unsigned framesRead = _provider->read(_next.ptr(), _bufferSize);

    if (framesRead < _bufferSize) {
        _next.resize(framesRead);
    }

    _isPreloaded = true;
}

template<typename T>
void InputStream<T>::readAhead()
{
    preload();
    swap();
    preload();
}

template<typename T>
void InputStream<T>::swap()
{
    if (!_isPreloaded) {
        throw std::runtime_error("Attempt to swap non-preloaded buffer");
    }

    _current = _next;
    _isPreloaded = false;
}

template<typename T>
InputStream<T> &InputStream<T>::operator >>(Buffer<T> &buffer)
{
    if (!_isPreloaded) {
        readAhead();
    } else if (_preloadThread.joinable()) {
        _preloadThread.join();
    }

    buffer = _current;
    swap();

    _preloadThread = std::thread(&InputStream<T>::preload, this);
    return *this;
}

template<typename T>
InputStream<T> &InputStream<T>::operator >>(InputStreamInfo &info)
{
    auto &fi = *_provider->fileInfo();
    info.channels = fi.channels;
    info.formatMajor = fi.format.majorText;
    info.formatMinor = fi.format.minorText;
    info.frames = fi.frames;
    info.sampleRate = fi.sampleRate;
    info.sampleType = fi.sampleType;
    return *this;
}

template<typename T>
InputStream<T> &InputStream<T>::operator >>(InputStreamStrings &strings)
{
    auto &fi = *_provider->fileInfo();
    strings.clear();

    for (unsigned i = 0; i < IAudioFile::StringEntryCount; i++) {
        if (fi.strings[i]) {
            strings[static_cast<IAudioFile::StringEntry>(i)] = fi.strings[i];
        }
    }

    return *this;
}

SOUND_INSTANTIATE(InputStream)
