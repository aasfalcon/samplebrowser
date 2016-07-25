#ifndef FILE_H
#define FILE_H
#include "buffer.h"
#include "format.h"

#include <QException>
#include <QDate>
#include <QTime>

namespace Sound {

#define SAMPLE_INTERNAL double
#define SAMPLE_INTERNAL_DOUBLE

struct BroadcastInfo {
    QString
        description,
        originator,
        originatorReference,
        umid,
        codingHistory;
    QDate originationDate;
    QTime originationTime;
    size_t
        timeReferenceLow,
        timeReferenceHigh;
    int version;
};

enum LoopMode {
    None = 800,
    Forward,
    Backward,
    Alternating,
};

struct LoopInfo {
    int timeSigNum;
    int timeSigDen;
    LoopMode mode;
    int beats;
    float bpm;
    int rootKey;
};

struct Instrument {
    struct Loop {
        LoopMode mode;

        size_t
            start,
            end,
            count;
    };

    int gain;
    int
        basenote,
        detune,
        velocityLo,
        velocityHi,
        keyLo,
        keyHi;
    QList<Loop> loops;
};

struct CartInfo {
    struct Timer {
        QString usage;
        int value;
    };

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
    QDate
        startDate,
        endDate;
    QTime
        startTime,
        endTime;
    int levelReference;
    QList<Timer> postTimers;
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

typedef QList<Cue> CueList;

class FileError : public QException {
public:
    virtual QString toString();

private:
    QString _message;
    QString _path;
};

class SndfileError : public FileError {
public:
    Error(SndfileHandle handle);
    virtual QString toString();

private:
    int number;
    QString log;
    QString version;
};

class File
{
public:
    File(const QString &path);
    ~File();

private:
    Buffer<SAMPLE_INTERNAL> _buffer;
    SndfileHandle _handle;
    QString _path;
};

class Reader : public File {
    enum WavexAmbisonic {
        AmbisonicNone = SF_AMBISONIC_NONE,
        AmbisonicBFormat = SF_AMBISONIC_B_FORMAT,
        AmbisonicNotSupported = 0,
    };

    ~Reader();

    WavexAmbisonic ambisonic() const;
    double compressionLevel() const;
    const CartInfo *cartInfo() const;
    CueList cues() const;
    const Instrument *instrument() const;
    const LoopInfo *loop() const;
    bool rf64AutoDowngrade() const;
    double vbrEncodingQuality() const;

private:
    BroadcastInfo *_broadcastInfo;
    CartInfo *_cartInfo;
    CueList _cues;
    Instrument *_instrument;
    LoopInfo *_loopInfo;
};

class Writer : public Reader {
    File(const QString &path, Format format, int channels);

    void setAmbisonic(WavexAmbisonic value);
    void setCartInfo(const CartInfo &value) const;
    void setCompressionLevel(double value);
    void setCues(const CueList &cues);
    void setInstrument(const Instrument &value) const;
    void setLoop(const LoopInfo &value) const;
    void setRf64AutoDowngrade(bool value);
    void setVbrEncodingQuality(double value);

    void truncate();
};

} // namespace Sound

#endif // FILE_H
