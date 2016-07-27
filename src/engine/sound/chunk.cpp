#include <QByteArray>
#include <QDate>
#include <QTime>

#include "libsndfile/src/sndfile.h"

#include "chunk.h"
#include "error.h"

using namespace Sound;

#define CART_DATE_FORMAT "yyyy-MM-dd"
#define CART_DATE_SIZE 10
#define CART_TIME_FORMAT "hh:mm:ss"
#define CART_TIME_SIZE 8

#define BROADCAST_DATE_FORMAT "yyyy:MM:dd"
#define BROADCASE_DATE_SIZE 10
#define BROADCAST_TIME_FORMAT "hh:mm:ss"
#define BROADCAST_TIME_SIZE 8

#define SET_FIELD_DATA(__chunk) \
    QString set_field_chunk(#__chunk);

#define SET_FIELD(__dest, __member) \
    if (!set(__dest, __member, sizeof __dest)) { \
        throw sizeError(set_field_chunk, #__member, __member, sizeof __dest); \
    }

#define GET_FIELD(__member, __source) \
    __member = get(__source, sizeof __source);

// Chunk
QString Chunk::get(const char *source, int max)
{
    QString result;
    result.fromLocal8Bit(source, max);
    return result;
}

bool Chunk::set(char *dest, const QString &source, int max)
{
    QByteArray buffer = source.toLocal8Bit();
    int length = buffer.length();

    memcpy(dest, buffer.data(), std::min(length, max));

    if (length < max) {
        memset(&dest[length], 0, max - length);
    } else if (length > max) {
        return false;
    }

    return true;
}

ChunkError Chunk::sizeError(const QString &chunk, const QString &field,
                            const QString &value, int max)
{
    ChunkError error;
    error.chunk = chunk;
    error.field = field;
    error.value = value;
    error.message = QString("Value length exeeds %1 characters").arg(max);
    return error;
}

// BroadcastInfo
void BroadcastInfo::fromRaw(const QByteArray &data)
{
    const SF_BROADCAST_INFO *bi = (const SF_BROADCAST_INFO *)data.data();
    GET_FIELD(description, bi->description);
    GET_FIELD(originator, bi->originator);
    GET_FIELD(originatorReference, bi->originator_reference);

    umid = QByteArray(bi->umid, sizeof bi->umid);
    codingHistory = get(bi->coding_history, bi->coding_history_size);
    origination = QDateTime(
                QDate::fromString(get(bi->origination_date, BROADCASE_DATE_SIZE), BROADCAST_DATE_FORMAT),
                QTime::fromString(get(bi->origination_time, BROADCAST_TIME_SIZE), BROADCAST_TIME_FORMAT)
                );
    timeReference = (uint64_t) bi->time_reference_high << 32 | bi->time_reference_low;
    version = bi->version;
}

QByteArray BroadcastInfo::toRaw() const
{
    SET_FIELD_DATA(BroadcastInfo);
    int size = offsetof(SF_BROADCAST_INFO, coding_history) + codingHistory.length();
    QByteArray result(size, 0);
    SF_BROADCAST_INFO *bi = (SF_BROADCAST_INFO *)result.data();
    SET_FIELD(bi->description, description);
    SET_FIELD(bi->originator, originator);
    SET_FIELD(bi->originator_reference, originatorReference);

    if (umid.length() > 64) {
        throw sizeError(set_field_chunk, "umid", umid, sizeof bi->umid);
    }

    memcpy(bi->umid, umid.data(), umid.length());
    bi->coding_history_size = codingHistory.length();
    set(bi->coding_history, codingHistory, bi->coding_history_size);
    set(bi->origination_date, origination.date().toString(BROADCAST_DATE_FORMAT), BROADCASE_DATE_SIZE);
    set(bi->origination_time, origination.time().toString(BROADCAST_TIME_FORMAT), BROADCAST_TIME_SIZE);
    bi->time_reference_low = (uint32_t)timeReference;
    bi->time_reference_high = timeReference >> 32;
    bi->version = version;
    return result;
}

// LoopInfo
void LoopInfo::fromRaw(const QByteArray &data)
{
    const SF_LOOP_INFO *li = (const SF_LOOP_INFO *)data.data();
    timeSigNum = li->time_sig_num;
    timeSigDen = li->time_sig_den;
    beats = li->num_beats;
    rootKey = li->root_key;
    mode = (Mode)li->loop_mode;
    bpm = li->bpm;
}

QByteArray LoopInfo::toRaw() const
{
    QByteArray result(sizeof(SF_LOOP_INFO), 0);
    SF_LOOP_INFO *li = (SF_LOOP_INFO *)result.data();
    li->time_sig_num = timeSigNum;
    li->time_sig_den = timeSigDen;
    li->num_beats = beats;
    li->root_key = rootKey;
    li->loop_mode = mode;
    li->bpm = bpm;
    return result;
}

// Instrument
void Instrument::fromRaw(const QByteArray &data) {
    const SF_INSTRUMENT *in = (const SF_INSTRUMENT *)data.data();
    gain = in->gain;
    basenote = in->basenote;
    detune = in->detune;
    velocityLo = in->velocity_lo;
    velocityHi = in->velocity_hi;
    keyLo = in->key_lo;
    keyHi = in->key_hi;
    loops.clear();

    for (int i = 0; i < in->loop_count; i++) {
        Loop loop = {
            (LoopInfo::Mode)in->loops[i].mode,
            in->loops[i].start,
            in->loops[i].end,
            in->loops[i].count,
        };

        loops.append(loop);
    }
}

QByteArray Instrument::toRaw() const {
    int mainSize = offsetof(SF_INSTRUMENT, loops);
    int loopSize = offsetof(SF_INSTRUMENT, loops[1]) - mainSize;
    int size = mainSize + loops.count() * loopSize;
    QByteArray result(size, 0);
    SF_INSTRUMENT *in = (SF_INSTRUMENT *)result.data();

    in->gain = gain;
    in->basenote = basenote;
    in->detune = detune;
    in->velocity_lo = velocityLo;
    in->velocity_hi = velocityHi;
    in->key_lo = keyLo;
    in->key_hi = keyHi;
    in->loop_count = loops.count();

    for (int i = 0; i < in->loop_count; i++) {
        const Loop &loop = loops[i];
        in->loops[i].mode = loop.mode;
        in->loops[i].start = loop.start;
        in->loops[i].end = loop.end;
        in->loops[i].count = loop.count;
    }

    return result;
}

// CartInfo
void CartInfo::fromRaw(const QByteArray &data)
{
    const SF_CART_INFO *ci = (const SF_CART_INFO *)data.data();

    GET_FIELD(version, ci->version);
    GET_FIELD(title, ci->title);
    GET_FIELD(artist, ci->artist);
    GET_FIELD(cutId, ci->cut_id);
    GET_FIELD(version, ci->client_id);
    GET_FIELD(category, ci->category);
    GET_FIELD(classification, ci->classification);
    GET_FIELD(outCue, ci->out_cue);
    GET_FIELD(producerAppId, ci->producer_app_id);
    GET_FIELD(producerAppVersion, ci->producer_app_version);
    GET_FIELD(userDef, ci->user_def);
    GET_FIELD(url, ci->url);
    GET_FIELD(version, ci->version);

    tag = get(ci->tag_text, ci->tag_text_size);

    start = QDateTime(
                QDate::fromString(get(ci->start_date, CART_DATE_SIZE), CART_DATE_FORMAT),
                QTime::fromString(get(ci->end_date, CART_TIME_SIZE), CART_TIME_FORMAT)
                );
    end = QDateTime(
                QDate::fromString(get(ci->start_date, CART_DATE_SIZE), CART_DATE_FORMAT),
                QTime::fromString(get(ci->end_date, CART_TIME_SIZE), CART_TIME_FORMAT)
                );

    levelReference = ci->level_reference;
    postTimers.clear();

    for (int i = 0; i < 8; i++) {
        if (*((uint32_t *) ci->post_timers[i].usage)) {
            Timer timer;
            timer.usage = get(ci->post_timers[i].usage, 4);
            timer.value = ci->post_timers[i].value;
            postTimers.append(timer);
        }
    }
}

QByteArray CartInfo::toRaw() const
{
    SET_FIELD_DATA(CartInfo);
    int size = offsetof(SF_CART_INFO, tag_text) + tag.length();
    QByteArray result(size, 0);
    SF_CART_INFO *ci = (SF_CART_INFO *)result.data();

    SET_FIELD(ci->version, version);
    SET_FIELD(ci->title, title);
    SET_FIELD(ci->artist, artist);
    SET_FIELD(ci->cut_id, cutId);
    SET_FIELD(ci->client_id, version);
    SET_FIELD(ci->category, category);
    SET_FIELD(ci->classification, classification);
    SET_FIELD(ci->out_cue, outCue);
    SET_FIELD(ci->producer_app_id, producerAppId);
    SET_FIELD(ci->producer_app_version, producerAppVersion);
    SET_FIELD(ci->user_def, userDef);
    SET_FIELD(ci->url, url);
    SET_FIELD(ci->version, version);

    ci->tag_text_size = tag.length();
    set(ci->tag_text, tag, ci->tag_text_size);

    set(ci->start_date, start.date().toString(CART_DATE_FORMAT), CART_DATE_SIZE);
    set(ci->start_time, start.time().toString(CART_TIME_FORMAT), CART_TIME_SIZE);
    set(ci->end_date, end.date().toString(CART_DATE_FORMAT), CART_DATE_SIZE);
    set(ci->end_time, end.time().toString(CART_TIME_FORMAT), CART_TIME_SIZE);

    ci->level_reference = levelReference;

    int count = postTimers.count();

    if (count > 8) {
        ChunkError error;
        error.chunk = set_field_chunk;
        error.field = "postTimers";
        error.message = "Post timer count exceeds limit of 8";
        error.value = count;
        throw error;
    }

    for (int i = 0; i < count; i++) {
        const Timer &timer = postTimers[i];

        if (!set(ci->post_timers[i].usage, timer.usage, 4)) {
            throw sizeError(set_field_chunk, QString("postTimers[%1].usage").arg(i),
                            timer.usage, 4);
        }

        ci->post_timers[i].value = timer.value;
    }

    return result;
}

// Cues
void Cues::fromRaw(const QByteArray &data)
{
    const SF_CUES *cs = (const SF_CUES *)data.data();
    clear();

    for (size_t i = 0; i < cs->cue_count; i++) {
        Cue point;
        point.indx = cs->cue_points[i].indx;
        point.fccChunk = cs->cue_points[i].fcc_chunk;
        point.chunkStart = cs->cue_points[i].chunk_start;
        point.blockStart = cs->cue_points[i].block_start;
        point.position = cs->cue_points[i].position;
        point.sampleOffset = cs->cue_points[i].sample_offset;
        point.name = get(cs->cue_points[i].name, sizeof cs->cue_points[i].name);
        append(point);
    }
}

QByteArray Cues::toRaw() const
{
    int size = offsetof(SF_CUES, cue_points) + count() * sizeof(SF_CUE_POINT);
    QByteArray result(size, 0);
    SF_CUES *cs = (SF_CUES *)result.data();
    cs->cue_count = count();

    for (int i = 0; i < count(); i++) {
        const Cue &point = at(i);
        cs->cue_points[i].indx = point.indx;
        cs->cue_points[i].fcc_chunk = point.fccChunk;
        cs->cue_points[i].chunk_start = point.chunkStart;
        cs->cue_points[i].block_start = point.blockStart;
        cs->cue_points[i].position = point.position;
        cs->cue_points[i].sample_offset = point.sampleOffset;

        int max = sizeof cs->cue_points[i].name;

        if (!set(cs->cue_points[i].name, point.name, max)) {
            sizeError("Cues", QString("[%1].name").arg(i), point.name, max);
        }
    }

    return result;
}
