#ifndef CONSTFRAME_H
#define CONSTFRAME_H

#include "buffer.h"
#include "frame.h"
#include "sample.h"
#include "sound.h"

template<typename T>
class ConstFrame: public Sound::Object<T>
{
    friend class Frame<T>;
public:
    ConstFrame(const Buffer<T> &buffer);
    ConstFrame(const Buffer<T> &buffer, unsigned offset);
    ConstFrame(const ConstFrame &source);
    ~ConstFrame() {}

    Sample<T> at(unsigned channel) const;
    unsigned channels() const;

    ConstFrame<T> &operator++();
    bool operator==(const ConstFrame<T> &rht) const;
    bool operator==(const Frame<T> &rht) const;
    bool operator!=(const ConstFrame<T> &rht) const;
    bool operator!=(const Frame<T> &rht) const;

private:
    unsigned _channels;
    const Sample<T> *_data;
};

#include "constframe.tcc"

#endif // CONSTFRAME_H
