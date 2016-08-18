#ifndef CONSTFRAME_H
#define CONSTFRAME_H

#include "buffer.h"
#include "frame.h"
#include "sample.h"
#include "sound.h"

template<typename T>
class ConstFrame: public Sound::Object<T>
{
public:
    ConstFrame(const Buffer<T> &buffer, unsigned offset = 0);
    ConstFrame(unsigned channels, const Sample<T> *data);
    ConstFrame(const ConstFrame &source);
    ConstFrame(const Frame<T> &source);
    ~ConstFrame() {}

    Sample<T> at(unsigned channel) const;
    unsigned channels() const;
    const Sample<T> *data() const;
    unsigned size() const;

    ConstFrame<T> &operator ++();
    ConstFrame<T> &operator --();
    int operator -(ConstFrame<T> rht) const;
    ConstFrame<T> operator +(int rht) const;
    ConstFrame<T> operator -(int rht) const;
    ConstFrame<T> &operator +=(int rht);
    ConstFrame<T> &operator -=(int rht);
    bool operator ==(const ConstFrame<T> &rht) const;
    bool operator !=(const ConstFrame<T> &rht) const;
    Sample<T> operator [](unsigned channel) const;

private:
    unsigned _channels;
    const Sample<T> *_data;
};

#include "constframe.tcc"

#endif // CONSTFRAME_H
