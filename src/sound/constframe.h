#ifndef CONSTFRAME_H
#define CONSTFRAME_H

#include "sound.h"

template<typename T> class Buffer;
template<typename T> class Frame;
template<typename T> class Sample;

template<typename T>
class ConstFrame: public Object<T>
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

SOUND_INSTANTIATE_DECLARATION(ConstFrame)

#endif // CONSTFRAME_H
