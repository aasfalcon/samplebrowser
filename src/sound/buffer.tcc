#ifndef SOUND_BUFFER_TCC
#define SOUND_BUFFER_TCC

#include <cstring>

#ifndef SOUND_BUFFER_H
#include "buffer.h"
#endif

#include "constframe.h"
#include "frame.h"
#include "shared/log.h"

namespace Sound {

template <typename T>
template <typename S>
Frame<T> Buffer<T>::copy(Frame<T> dbeg, Frame<T> dend,
    ConstFrame<S> sbeg, ConstFrame<S> send)
{
    int dcount = dend - dbeg;
    int scount = send - sbeg;
    int count = std::min(dcount, scount);

    if (!count) {
        return dbeg;
    }

    ERROR_IF(dcount < 0 || scount < 0 || scount > dcount,
        RUNTIME_ERROR, "Invalid buffer copy parameters");

    if (dbeg.type() == sbeg.type() && dbeg.channels() == sbeg.channels()) {
        std::memcpy(dbeg.data(), sbeg.data(), count * sbeg.channels() * sizeof(T));
        return dbeg + count;
    }

    unsigned sch = sbeg.channels();
    unsigned dch = dbeg.channels();
    Frame<T> dit;
    ConstFrame<S> sit;

    if (1 == dch && 1 < sch) {
        for (dit = dbeg, sit = sbeg; sit != send; ++sit, ++dit) {
            Sample<Precise> left = sit[0];
            Sample<Precise> right = sit[1];
            Sample<Precise> mix((left.value() + right.value()) / 2);
            dit[0] = mix;
        }
    } else if (2 == dch && 1 == sch) {
        for (dit = dbeg, sit = sbeg; sit != send; ++sit, ++dit) {
            dit[0] = dit[1] = sit[0];
        }
    } else {
        unsigned common = std::min(sch, dch);
        unsigned channel = 0;

        for (; channel < common; channel++) {
            for (dit = dbeg, sit = sbeg; sit != send; ++sit, ++dit) {
                dit[channel] = sit[channel];
            }
        }

        for (; channel < dch; channel++) {
            for (dit = dbeg; dit != dend; ++dit) {
                dit[channel] = dit.nil();
            }
        }
    }

    return dit;
}
}
#endif // SOUND_BUFFER_TCC
