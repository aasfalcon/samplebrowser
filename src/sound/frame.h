#ifndef FRAME_H
#define FRAME_H

#include "buffer.h"
#include "sample.h"
#include "sound.h"

template <typename T>
class ConstFrame;

template <typename T>
class Frame : public Sound::Object<T> {
public:
    Frame(unsigned channels, Sample<T>* ptr);
    Frame(const Frame& source);
    ~Frame() {}

    Sample<T> at(unsigned channel) const;
    unsigned channels() const;
    const Sample<T>* data() const;
    Sample<T>* data();
    const T* ptr() const;
    T* ptr();
    unsigned size() const;

    Frame<T>& operator++();
    Frame<T>& operator--();
    int operator-(Frame<T> rht) const;
    Frame<T> operator+(int rht) const;
    Frame<T> operator-(int rht) const;
    Frame<T>& operator+=(int rht);
    Frame<T>& operator-=(int rht);

    template <typename S>
    Frame<T>& operator=(ConstFrame<S> rht);

    bool operator==(const ConstFrame<T>& rht) const;
    bool operator!=(const ConstFrame<T>& rht) const;
    Sample<T> operator[](unsigned channel) const;
    Sample<T>& operator[](unsigned channel);

    void put(unsigned channel, Sample<T> value);

private:
    unsigned _channels;
    Sample<T>* _data;
};

SOUND_INSTANTIATION_DECLARE(Frame);

#endif // FRAME_H
