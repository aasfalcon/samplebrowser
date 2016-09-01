#ifndef MIXER_H
#define MIXER_H

#include <vector>

#include "buffer.h"
#include "config.h"
#include "constframe.h"
#include "frame.h"
#include "object.h"

namespace Sound {

template <typename T>
class Mixer : public Object<T> {
public:
    Mixer();

    void add(ConstFrame<T> sbeg, ConstFrame<T> send);

    void clear();

    Frame<T> mixTo(Frame<T> dbeg, Frame<T> dend) const;

private:
    unsigned _count;

    struct Source {
        unsigned channels;
        const Sample<T>* data;
        unsigned frames;
    };

    std::vector<Source> _sources;
};

SOUND_INSTANTIATION_DECLARE(Mixer);
}

#endif // MIXER_H
