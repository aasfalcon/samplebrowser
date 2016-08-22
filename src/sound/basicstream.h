#ifndef SOUND_BASICSTREAM_H
#define SOUND_BASICSTREAM_H

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "chunk.h"
#include "shared/iaudiofile.h"

namespace Sound {

class BasicStream {
public:
    class Eof : public std::exception {
    public:
        Eof(unsigned tail);
        unsigned tail();

    private:
        unsigned _tail;
    };

    struct Info {
        unsigned channels;
        unsigned frames;
        Type sampleType;
        unsigned sampleRate;
    };

    struct FormatText {
        std::string majorText;
        std::string minorText;
    };

    struct RawChunk {
        std::string id;
        std::vector<IAudioFile::ChunkData> data;
    };

    typedef IAudioFile::CompressionInfo Compression;
    typedef IAudioFile::Format Format;
    typedef std::map<IAudioFile::StringEntry, std::string> Strings;
    typedef std::vector<RawChunk> RawChunks;

    bool isSeekable() const;
    std::string path() const;
    unsigned pos(IAudioFile::SeekType type) const;
    void seek(int pos, IAudioFile::SeekWhence whence,
        IAudioFile::SeekType type);

protected:
    IAudioFile::FileInfo _fi;
    std::string _path;
    std::shared_ptr<IAudioFile> _provider;
    RawChunks _rawChunks;

    BasicStream();
    virtual ~BasicStream() = 0;

    void open(const std::string& path, IAudioFile::Mode mode);
};

class BasicInputStream : virtual protected BasicStream {
public:
    bool eof() const;

protected:
    BasicInputStream();
    ~BasicInputStream();

    void read(Compression& compression);
    void read(Chunk& chunk);
    void read(Format& format);
    void read(FormatText& formatText);
    void read(Info& info);
    void read(Strings& strings);
    void read(RawChunks& rawChunks);
    void read(void* data, Type type, unsigned frames);
};

class BasicOutputStream : virtual protected BasicStream {
protected:
    std::vector<IAudioFile::RawChunk> _rawChunks;

    BasicOutputStream();
    ~BasicOutputStream();

    void write(const Compression& compression);
    void write(const Chunk& chunk);
    void write(const Format& format);
    void write(const Info& info);
    void write(const Strings& strings);
    void write(const RawChunks& rawChunks);
    void write(const RawChunk& rawChunk);

    void outputStart();

private:
    bool _isOutputStarted;
};

class BasicIOStream
    : virtual protected BasicInputStream,
      virtual protected BasicOutputStream,
      virtual public BasicStream {
public:
    unsigned pos() const;
    void seek(int pos, IAudioFile::SeekWhence whence);

protected:
    BasicIOStream();
    ~BasicIOStream();
};
}

#endif // SOUND_BASICSTREAM_H
