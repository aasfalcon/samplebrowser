#include <cstddef>
#include <cstring>
#include <iomanip>
#include <sstream>

#include "libsndfile/build/src/sndfile.h"
#include "chunk.h"

#define CART_DATE_FORMAT "Y-m-d"
#define CART_TIME_FORMAT "H:M:S"

#define BROADCAST_DATE_FORMAT "Y:m:d"
#define BROADCAST_TIME_FORMAT "H:M:S"

using namespace Sound;

// Chunk
void Chunk::copy(char *dest, const std::string &source, unsigned max)
{
    memset(dest, 0, int(max));
    memcpy(dest, source.data(), int(std::min(source.length(), max)));
}

// BroadcastInfo
void BroadcastInfo::fromRaw(const Chunk::RawData &data)
{
    auto bi = reinterpret_cast<const SF_BROADCAST_INFO *>(data.data());
    description.assign(bi->description, sizeof bi->description);
    originator.assign(bi->originator, sizeof bi->originator);
    originatorReference.assign(bi->originator_reference,
                               sizeof bi->originator_reference);

    umid.assign(bi->umid, sizeof bi->umid);
    codingHistory.assign(bi->coding_history, bi->coding_history_size);

    std::string date(bi->origination_date, sizeof bi->origination_date);
    std::string time(bi->origination_time, sizeof bi->origination_time);
    std::string format(BROADCAST_DATE_FORMAT + std::string("|")
                       + BROADCAST_TIME_FORMAT);

    std::stringstream ss(date + '|' + time);
    ss >> std::get_time(&origination, format.c_str());

    timeReference = static_cast<unsigned long long>(bi->time_reference_high);
    timeReference <<= 32;
    timeReference |= bi->time_reference_low;
    version = bi->version;
}

Chunk::RawData BroadcastInfo::toRaw() const
{
    unsigned size = offsetof(SF_BROADCAST_INFO, coding_history)
            + codingHistory.length();
    Chunk::RawData result(size, 0);
    auto bi = reinterpret_cast<SF_BROADCAST_INFO *>(result.data());

    copy(bi->description, description, sizeof bi->description);
    copy(bi->originator, originator, sizeof bi->originator);
    strncpy(bi->originator_reference, originatorReference.c_str(),
            sizeof bi->originator_reference);
    copy(bi->umid, umid, sizeof bi->umid);

    bi->coding_history_size = codingHistory.length();
    strncpy(bi->coding_history, codingHistory.c_str(),
            int(bi->coding_history_size));

    std::ostringstream date, time;
    date << std::put_time(&origination, BROADCAST_DATE_FORMAT);
    time << std::put_time(&origination, BROADCAST_TIME_FORMAT);
    copy(bi->origination_date, date.str(), sizeof bi->origination_date);
    copy(bi->origination_time, time.str(), sizeof bi->origination_time);

    bi->time_reference_low = unsigned(timeReference);
    bi->time_reference_high = unsigned(timeReference >> 32);
    bi->version = short(version);
    return result;
}

// LoopInfo
void LoopInfo::fromRaw(const Chunk::RawData &raw)
{
    auto li = reinterpret_cast<const SF_LOOP_INFO *>(raw.data());
    timeSigNum = li->time_sig_num;
    timeSigDen = li->time_sig_den;
    beats = li->num_beats;
    rootKey = li->root_key;
    mode = static_cast<Mode>(li->loop_mode);
    bpm = li->bpm;
}

Chunk::RawData LoopInfo::toRaw() const
{
    Chunk::RawData result(sizeof(SF_LOOP_INFO), 0);
    auto li = reinterpret_cast<SF_LOOP_INFO *>(result.data());
    li->time_sig_num = short(timeSigNum);
    li->time_sig_den = short(timeSigDen);
    li->num_beats = beats;
    li->root_key = rootKey;
    li->loop_mode = mode;
    li->bpm = bpm;
    return result;
}

// Instrument
void Instrument::fromRaw(const Chunk::RawData &raw) {
    auto is = reinterpret_cast<const SF_INSTRUMENT *>(raw.data());
    gain = is->gain;
    basenote = is->basenote;
    detune = is->detune;
    velocityLo = is->velocity_lo;
    velocityHi = is->velocity_hi;
    keyLo = is->key_lo;
    keyHi = is->key_hi;
    loops.clear();

    for (int i = 0; i < is->loop_count; i++) {
        Loop loop = {
            static_cast<LoopInfo::Mode>(is->loops[i].mode),
            is->loops[i].start,
            is->loops[i].end,
            is->loops[i].count,
        };

        loops.push_back(loop);
    }
}

Chunk::RawData Instrument::toRaw() const {
    unsigned mainSize = offsetof(SF_INSTRUMENT, loops);
    unsigned loopSize = offsetof(SF_INSTRUMENT, loops[1]) - mainSize;
    unsigned size = mainSize + loops.size() * loopSize;
    Chunk::RawData result(size, 0);
    auto is = reinterpret_cast<SF_INSTRUMENT *>(result.data());

    is->gain = gain;
    is->basenote = char(basenote);
    is->detune = char(detune);
    is->velocity_lo = char(velocityLo);
    is->velocity_hi = char(velocityHi);
    is->key_lo = char(keyLo);
    is->key_hi = char(keyHi);
    is->loop_count = int(loops.size());
    auto loop = loops.cbegin();

    for (int i = 0; i < is->loop_count; i++) {
        is->loops[i].mode = loop->mode;
        is->loops[i].start = loop->start;
        is->loops[i].end = loop->end;
        is->loops[i].count = loop->count;
        ++loop;
    }

    return result;
}

// CartInfo
void CartInfo::fromRaw(const Chunk::RawData &raw)
{
    auto ci = reinterpret_cast<const SF_CART_INFO *>(raw.data());

    version.assign(ci->version, sizeof ci->version);
    title.assign(ci->title, sizeof ci->title);
    artist.assign(ci->artist, sizeof ci->artist);
    cutId.assign(ci->cut_id, sizeof ci->cut_id);
    version.assign(ci->client_id, sizeof ci->client_id);
    category.assign(ci->category, sizeof ci->category);
    classification.assign(ci->classification, sizeof ci->classification);
    outCue.assign(ci->out_cue, sizeof ci->out_cue);
    producerAppId.assign(ci->producer_app_id, sizeof ci->producer_app_id);
    producerAppVersion.assign(ci->producer_app_version, sizeof ci->producer_app_version);
    userDef.assign(ci->user_def, sizeof ci->user_def);
    url.assign(ci->url, sizeof ci->url);
    version.assign(ci->version, sizeof ci->version);

    tag.assign(ci->tag_text, ci->tag_text_size);

    std::string format(CART_DATE_FORMAT + std::string("|")
                       + CART_TIME_FORMAT);
    std::string date(ci->start_date, sizeof ci->start_date);
    std::string time(ci->start_time, sizeof ci->start_time);
    std::stringstream ss(date + '|' + time);
    ss >> std::get_time(&start, format.c_str());

    date.assign(ci->end_date, sizeof ci->end_date);
    time.assign(ci->end_time, sizeof ci->end_time);
    ss = std::stringstream(date + '|' + time);
    ss >> std::get_time(&end, format.c_str());

    levelReference = ci->level_reference;
    postTimers.clear();

    for (int i = 0; i < 8; i++) {
        if (reinterpret_cast<int>(ci->post_timers[i].usage)) {
            Timer timer;
            timer.usage.assign(ci->post_timers[i].usage,
                               sizeof ci->post_timers[i].usage);
            timer.value = ci->post_timers[i].value;
            postTimers.push_back(timer);
        }
    }
}

Chunk::RawData CartInfo::toRaw() const
{
    unsigned size = offsetof(SF_CART_INFO, tag_text) + tag.length();
    Chunk::RawData result(size, 0);
    auto ci = reinterpret_cast<SF_CART_INFO *>(result.data());

    copy(ci->version, version, sizeof ci->version);
    copy(ci->title, title, sizeof ci->title);
    copy(ci->artist, artist, sizeof ci->artist);
    copy(ci->cut_id, cutId, sizeof ci->cut_id);
    copy(ci->client_id, version, sizeof ci->client_id);
    copy(ci->category, category, sizeof ci->category);
    copy(ci->classification, classification, sizeof ci->classification);
    copy(ci->out_cue, outCue, sizeof ci->out_cue);
    copy(ci->producer_app_id, producerAppId, sizeof ci->producer_app_id);
    copy(ci->producer_app_version, producerAppVersion, sizeof ci->producer_app_version);
    copy(ci->user_def, userDef, sizeof ci->user_def);
    copy(ci->url, url, sizeof ci->url);
    copy(ci->version, version, sizeof ci->version);

    ci->tag_text_size = tag.length();
    copy(ci->tag_text, tag, sizeof ci->tag_text);

    std::ostringstream start_date, start_time;
    start_date << std::put_time(&start, CART_DATE_FORMAT);
    start_time << std::put_time(&start, CART_TIME_FORMAT);
    copy(ci->start_date, start_date.str(), sizeof ci->start_date);
    copy(ci->start_time, start_time.str(), sizeof ci->start_time);

    std::ostringstream end_date, end_time;
    end_date << std::put_time(&end, CART_DATE_FORMAT);
    end_time << std::put_time(&end, CART_TIME_FORMAT);
    copy(ci->end_date, end_date.str(), sizeof ci->end_date);
    copy(ci->end_time, end_time.str(), sizeof ci->end_time);

    ci->level_reference = levelReference;

    unsigned count = std::min(postTimers.size(), 8u);
    auto timer = postTimers.cbegin();

    for (unsigned i = 0; i < count; i++) {
        copy(ci->post_timers[i].usage, timer->usage, sizeof ci->post_timers[i].usage);
        ci->post_timers[i].value = timer->value;
        ++timer;
    }

    return result;
}

// Cues
void Cues::fromRaw(const Chunk::RawData &raw)
{
    auto cs = reinterpret_cast<const SF_CUES *>(raw.data());
    clear();

    for (unsigned i = 0; i < cs->cue_count; i++) {
        Cue point;
        point.indx = cs->cue_points[i].indx;
        point.fccChunk = cs->cue_points[i].fcc_chunk;
        point.chunkStart = cs->cue_points[i].chunk_start;
        point.blockStart = cs->cue_points[i].block_start;
        point.position = cs->cue_points[i].position;
        point.sampleOffset = cs->cue_points[i].sample_offset;
        point.name.assign(cs->cue_points[i].name,
                          sizeof cs->cue_points[i].name);
        push_back(point);
    }
}

Chunk::RawData Cues::toRaw() const
{
    unsigned raw_size = offsetof(SF_CUES, cue_points)
            + size() * sizeof(SF_CUE_POINT);
    Chunk::RawData result(raw_size, 0);
    auto cs = reinterpret_cast<SF_CUES *>(result.data());
    cs->cue_count = size();
    auto point = cbegin();

    for (unsigned i = 0; i < cs->cue_count; i++) {
        cs->cue_points[i].indx = point->indx;
        cs->cue_points[i].fcc_chunk = point->fccChunk;
        cs->cue_points[i].chunk_start = point->chunkStart;
        cs->cue_points[i].block_start = point->blockStart;
        cs->cue_points[i].position = point->position;
        cs->cue_points[i].sample_offset = point->sampleOffset;

        unsigned max = sizeof cs->cue_points[i].name;
        copy(cs->cue_points[i].name, point->name, max);

        ++point;
    }

    return result;
}
