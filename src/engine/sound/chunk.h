#ifndef CHUNK_H
#define CHUNK_H

#include <QByteArray>
#include <QDateTime>
#include <QString>

#include <stdint.h>

namespace Sound {

class ChunkError;

struct Chunk {
public:
    virtual ~Chunk() {}

    template<class T>
    inline static T create(const QByteArray &data) {
        T chunk;
        chunk.fromRaw(data);
        return chunk;
    }

    virtual void fromRaw(const QByteArray &data) = 0;
    virtual QByteArray toRaw() const = 0;

protected:
    static QString get(const char *source, int max);
    static bool set(char *dest, const QString &source, int max);
    static ChunkError sizeError(const QString &chunk, const QString &field,
                                const QString &value, int max);
};

struct BroadcastInfo : public Chunk {
    QString
        description,
        originator,
        originatorReference,
        codingHistory;
    QByteArray umid;
    QDateTime origination;
    uint64_t timeReference;
    uint16_t version;

protected:
    void fromRaw(const QByteArray &data);
    QByteArray toRaw() const;
};

struct LoopInfo : public Chunk {
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
    void fromRaw(const QByteArray &data);
    QByteArray toRaw() const;
};

struct Instrument : public Chunk {
    struct Loop {
        LoopInfo::Mode mode;

        size_t
            start,
            end,
            count;
    };

    typedef QList<Loop> Loops;

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
    void fromRaw(const QByteArray &data);
    QByteArray toRaw() const;
};

struct CartInfo : public Chunk {
    struct Timer {
        QString usage;
        int value;
    };

    typedef QList<Timer> Timers;

    QString
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
    QDateTime start;
    QDateTime end;
    int levelReference;
    Timers postTimers;

protected:
    void fromRaw(const QByteArray &data);
    QByteArray toRaw() const;
};

struct Cue {
    int
        indx,
        fccChunk,
        chunkStart,
        blockStart;

    size_t
        position,
        sampleOffset;

    QString name;
};

struct Cues : public Chunk, QList<Cue> {
protected:
    void fromRaw(const QByteArray &data);
    QByteArray toRaw() const;
};

} // namespace Sound

#endif // CHUNK_H
