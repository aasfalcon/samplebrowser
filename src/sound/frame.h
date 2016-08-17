#ifndef FRAME_H
#define FRAME_H

#include "buffer.h"
#include "constframe.h"
#include "sample.h"
#include "sound.h"

template<typename T>
class ConstFrame;

template<typename T>
class Frame: public Sound::Object<T>
{
    friend class ConstFrame<T>;
public:
    Frame(Buffer<T> &buffer);
    Frame(Buffer<T> &buffer, unsigned offset);
    Frame(const Frame &frame);
    ~Frame() {}

    Sample<T> at(unsigned channel) const;
    unsigned channels() const;
    void put(unsigned channel, Sample<T> value);

    Frame<T> &operator++();
    bool operator==(const Frame<T> &rht) const;
    bool operator==(const ConstFrame<T> &rht) const;
    bool operator!=(const Frame<T> &rht) const;
    bool operator!=(const ConstFrame<T> &rht) const;

private:
    unsigned _channels;
    Sample<T> *_data;
};

#include "frame.tcc"

#endif // FRAME_H
