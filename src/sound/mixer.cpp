#include "mixer.h"

using namespace Sound;

template <typename T>
Mixer<T>::Mixer()
    : _count(0)
    , _sources(SOUND_MIXER_MAX_SOURCES)
{
}

template <typename T>
void Mixer<T>::add(ConstFrame<T> sbeg, ConstFrame<T> send)
{
    ERROR_IF(_count + 1 > SOUND_MIXER_MAX_SOURCES, RUNTIME_ERROR,
        "Too big number of mixer sources");

    int frames = send - sbeg;

    ERROR_IF(frames < 0 || send.channels() != sbeg.channels(), RUNTIME_ERROR,
        "Invalid source parameters");

    _sources[_count++] = { sbeg.channels(), sbeg.samples(), unsigned(frames) };
}

template <typename T>
void Mixer<T>::clear()
{
    _count = 0;
}

template <typename T>
Frame<T> Mixer<T>::mixTo(Frame<T> dbeg, Frame<T> dend) const
{
    if (dbeg == dend) {
        return dbeg;
    }

    ERROR_IF(dend - dbeg < 0 || dend.channels() != dbeg.channels(), RUNTIME_ERROR,
        "Invalid destination parameters");

    unsigned dch = dbeg.channels();
    unsigned frames = unsigned(dend - dbeg);
    Sample<T>* dest = dbeg.samples();

    for (unsigned f = 0; f < frames; f++) {
        for (unsigned c = 0; c < dch; c++) {
            Precise value = NIL_PRECISE;

            for (unsigned s = 0; s < _count; s++) {
                if (_sources[s].frames >= f) {
                    continue;
                }

                unsigned sch = _sources[s].channels;

                if (c < 2) {
                    if (1 == sch && 1 < dch) {
                        value += Sample<Precise>(_sources[s].data[0]).value();
                    } else if (1 == dch && 1 < sch) {
                        value += Sample<Precise>(_sources[s].data[c]).value() / 2;
                    }
                } else if (c < sch) {
                    value += Sample<Precise>(_sources[s].data[c]).value();
                }
            }

            dest[f * dch + c] = Sample<Precise>(value / _count);
        }
    }

    unsigned framesMax = 0;

    for (unsigned i = 0; i < _count; i++) {
        if (_sources[i].frames > framesMax) {
            framesMax = _sources[i].frames;
        }
    }

    return dbeg + int(std::max(frames, framesMax));
}

SOUND_INSTANTIATE(Sound::Mixer);
