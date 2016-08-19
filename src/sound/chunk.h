#ifndef CHUNK_H
#define CHUNK_H

#include <ctime>
#include <list>
#include <string>
#include <vector>

namespace Sound {

struct Chunk {
    friend class BasicInputStream;
    friend class BasicOutputStream;

    bool exists;

public:
    typedef std::vector<unsigned char> RawData;

    virtual ~Chunk() {}

    template <class T>
    inline static T create(const RawData& data)
    {
        T chunk;
        chunk.fromRaw(data);
        return chunk;
    }

protected:
    static void copy(char* dest, const std::string& source, unsigned max);
    virtual void fromRaw(const RawData& data) = 0;
    virtual RawData toRaw() const = 0;
};

struct BroadcastInfo : Chunk {
    std::string
        description,
        originator,
        originatorReference,
        codingHistory;
    std::string umid;
    std::tm origination;
    unsigned long long timeReference;
    int version;

protected:
    void fromRaw(const RawData& data);
    RawData toRaw() const;
};

struct LoopInfo : Chunk {
    enum Mode {
        None = 800,
        Forward,
        Backward,
        Alternating,
    };

    int
        timeSigNum,
        timeSigDen,
        beats,
        rootKey;
    Mode mode;
    float bpm;

protected:
    void fromRaw(const RawData& data);
    RawData toRaw() const;
};

struct Instrument : Chunk {
    struct Loop {
        LoopInfo::Mode mode;

        unsigned
            start,
            end,
            count;
    };

    typedef std::list<Loop> Loops;

    int
        gain,
        basenote,
        detune,
        velocityLo,
        velocityHi,
        keyLo,
        keyHi;

    Loops loops;

protected:
    void fromRaw(const RawData& data);
    RawData toRaw() const;
};

struct CartInfo : public Chunk {
    struct Timer {
        std::string usage;
        int value;
    };

    typedef std::list<Timer> Timers;

    std::string
        version,
        title,
        artist,
        cutId,
        clientId,
        category,
        classification,
        outCue,
        producerAppId,
        producerAppVersion,
        userDef,
        url,
        tag;
    std::tm start;
    std::tm end;
    int levelReference;
    Timers postTimers;

protected:
    void fromRaw(const RawData& data);
    RawData toRaw() const;
};

struct Cue {
    int
        indx,
        fccChunk,
        chunkStart,
        blockStart;

    unsigned
        position,
        sampleOffset;

    std::string name;
};

struct Cues : Chunk, std::list<Cue> {
protected:
    void fromRaw(const RawData& data);
    RawData toRaw() const;
};
}

#endif // CHUNK_H
