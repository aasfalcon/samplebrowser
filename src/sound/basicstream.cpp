#include "basicstream.h"
#include "shared/log.h"
#include "shared/server.h"

using namespace Sound;

// BasicStream::Eof
BasicStream::Eof::Eof(unsigned tail)
    : _tail(tail)
{
}

unsigned BasicStream::Eof::tail()
{
    return _tail;
}

// BasicStream
BasicStream::BasicStream()
    : _provider(PLUGIN_FACTORY(IAudioFile))
{
}

BasicStream::~BasicStream()
{
}

bool BasicStream::isSeekable() const
{
    return _provider->fileInfo()->seekable;
}

std::string BasicStream::path() const
{
    return _path;
}

unsigned BasicStream::pos(IAudioFile::SeekType type) const
{
    return _provider->seek(0, IAudioFile::SeekCur, type);
}

void BasicStream::seek(int pos, IAudioFile::SeekWhence whence,
    IAudioFile::SeekType type)
{
    _provider->seek(pos, whence, type);
}

void BasicStream::open(const std::string& path, IAudioFile::Mode mode)
{
    _path = path;
    _provider->open(_path.c_str(), mode);
}

// BasicInputStream
BasicInputStream::BasicInputStream()
{
}

BasicInputStream::~BasicInputStream()
{
}

bool BasicInputStream::eof() const
{
    return _provider->seek(0, IAudioFile::SeekCur, IAudioFile::SeekTypeRead)
        == _provider->fileInfo()->frames;
}

void BasicInputStream::read(BasicStream::Compression& compression)
{
    auto& fi = *_provider->fileInfo();
    compression = fi.compression;
}

void BasicInputStream::read(Chunk& chunk)
{
    const IAudioFile::ChunkData* ptr = nullptr;
    unsigned size = 0;

    if (dynamic_cast<BroadcastInfo*>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkBroadcastInfo, &size);
    } else if (dynamic_cast<LoopInfo*>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkLoopInfo, &size);
    } else if (dynamic_cast<Instrument*>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkInstrument, &size);
    } else if (dynamic_cast<CartInfo*>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkCartInfo, &size);
    } else if (dynamic_cast<Cues*>(&chunk)) {
        ptr = _provider->chunk(IAudioFile::ChunkCues, &size);
    }

    if (ptr && size) {
        Chunk::RawData data;
        data.assign(ptr, ptr + size);
        chunk.fromRaw(data);
    }
}

void BasicInputStream::read(BasicStream::Format& format)
{
    auto& fi = *_provider->fileInfo();
    format = fi.format;
}

void BasicInputStream::read(BasicStream::FormatText& formatText)
{
    Format format;
    read(format);
    auto pft = _provider->formatText(format);
    formatText.majorText = pft.majorText;
    formatText.minorText = pft.minorText;
}

void BasicInputStream::read(BasicStream::Info& info)
{
    auto& fi = *_provider->fileInfo();
    info.channels = fi.channels;
    info.frames = fi.frames;
    info.sampleRate = fi.sampleRate;
    info.sampleType = fi.sampleType;
}

void BasicInputStream::read(BasicStream::Strings& strings)
{
    strings.clear();

    for (unsigned i = 0; i < IAudioFile::StringEntryCount; i++) {
        auto entry = IAudioFile::StringEntry(i);
        const char* string = _provider->string(entry);

        if (string) {
            strings[entry] = string;
        }
    }
}

void BasicInputStream::read(RawChunks& rawChunks)
{
    unsigned count;
    auto chunks = _provider->rawChunks(&count);
    rawChunks.clear();

    for (unsigned i = 0; i < count; i++) {
        auto& prc = chunks[i];
        std::string id = prc.id;
        std::vector<IAudioFile::ChunkData> data;
        data.assign(prc.data, prc.data + prc.size);
        rawChunks.push_back({ id, data });
    }
}

void BasicInputStream::read(void* data, Type type, unsigned frames)
{
    unsigned framesRead = _provider->read(data, type, frames);

    if (framesRead < frames) {
        throw Eof(frames - framesRead);
    }
}

// BasicOutputStream
BasicOutputStream::BasicOutputStream()
    : _isOutputStarted(false)
{
}

BasicOutputStream::~BasicOutputStream()
{
}

void BasicOutputStream::write(const BasicStream::Compression& compression)
{
    if (_isOutputStarted) {
        std::string message = "Attempt to set compression data after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    _fi.compression = compression;
}

void BasicOutputStream::write(const Chunk& chunk)
{
    if (_isOutputStarted) {
        std::string message = "Attempt write chunk data after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    auto raw = chunk.toRaw();
    auto data = raw.data();
    auto size = raw.size();

    if (dynamic_cast<const BroadcastInfo*>(&chunk)) {
        _provider->setChunk(IAudioFile::ChunkBroadcastInfo, size, data);
    } else if (dynamic_cast<const LoopInfo*>(&chunk)) {
        _provider->setChunk(IAudioFile::ChunkLoopInfo, size, data);
    } else if (dynamic_cast<const Instrument*>(&chunk)) {
        _provider->setChunk(IAudioFile::ChunkInstrument, size, data);
    } else if (dynamic_cast<const CartInfo*>(&chunk)) {
        _provider->setChunk(IAudioFile::ChunkCartInfo, size, data);
    } else if (dynamic_cast<const Cues*>(&chunk)) {
        _provider->setChunk(IAudioFile::ChunkCues, size, data);
    } else {
        std::string message = "Unknown chunk type";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }
}

void BasicOutputStream::write(const BasicStream::Format& format)
{
    if (_isOutputStarted) {
        std::string message = "Attempt to set file format after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    _fi.format = format;
}

void BasicOutputStream::write(const BasicStream::Info& info)
{
    if (_isOutputStarted) {
        std::string message = "Attempt to set file sample info after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    _fi.channels = info.channels;
    _fi.frames = info.frames;
    _fi.sampleRate = info.sampleRate;
    _fi.sampleType = info.sampleType;
}

void BasicOutputStream::write(const BasicStream::Strings& strings)
{
    if (_isOutputStarted) {
        std::string message = "Attempt to write strings data after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    for (unsigned i = 0; i < IAudioFile::StringEntryCount; i++) {
        auto entry = IAudioFile::StringEntry(i);

        try {
            _provider->setString(entry, strings.at(entry).c_str());
        } catch (std::out_of_range) {
            _provider->setString(entry, nullptr);
        }
    }
}

void BasicOutputStream::write(const BasicStream::RawChunks& rawChunks)
{
    for (auto it = rawChunks.begin(); it != rawChunks.end(); it++) {
        write(*it);
    }
}

void BasicOutputStream::write(const BasicStream::RawChunk& rawChunk)
{
    if (_isOutputStarted) {
        std::string message = "Attempt to add raw chunk after output start";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    _rawChunks.push_back({ rawChunk.id.c_str(),
        rawChunk.data.data(),
        rawChunk.data.size() });
}

void BasicOutputStream::outputStart()
{
    _isOutputStarted = true;
    _provider->setFileInfo(&_fi);
    _provider->setRawChunks(_rawChunks.data(), _rawChunks.size());
}
