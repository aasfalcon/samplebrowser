#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

#include "sample.h"
#include "sound.h"

template<typename T>
class Frame;

template<typename T>
class ConstFrame;

template<typename T>
class Buffer: public Sound::Object<T>
{
public:
    Buffer();

    template<typename S>
    Buffer(ConstFrame<S> sbeg, ConstFrame<S> send);

    Buffer(unsigned channels, unsigned frames);

    template<typename S>
    void assign(ConstFrame<S> sbeg, ConstFrame<S> send);

    template<typename S>
    void assign(unsigned channels, const S *sbeg, const S *send);

    Frame<T> begin();
    ConstFrame<T> cbegin() const;
    ConstFrame<T> cend() const;
    unsigned channels() const;

    template<typename S>
    Frame<T> copyFrom(ConstFrame<S> sbeg, ConstFrame<S> send);

    Frame<T> end();
    unsigned frames() const;
    bool isEmpty() const;
    T *data();
    const T *data() const;
    void nativeInt24(void *dest);
    void reallocate(unsigned channels, unsigned frames);
    Buffer<T> resample(unsigned destRate, unsigned sourceRate,
                       Sound::Quality quality = Sound::QualityBest);
    void resize(unsigned frames);
    void resample(ConstFrame<T> sbeg, ConstFrame<T> send, unsigned destRate, unsigned sourceRate,
                  Sound::Quality quality = Sound::QualityBest);
    void silence();
    void silence(Frame<T> dbeg, Frame<T> dend);
    unsigned size() const;

private:
    unsigned _channels;
    unsigned _frames;
    std::vector<Sample<T>> _samples;
};

#include "buffer.tcc"

#endif // BUFFER_H
