#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include "sound.h"

template<typename T> class ConstFrame;
template<typename T> class Frame;
template<typename T> class Sample;

template<typename T>
class Buffer: public Object<T>
{
public:
    Buffer();
    Buffer(const Buffer<T> &source);
    Buffer(unsigned channels, unsigned frames);

    Frame<T> begin();
    ConstFrame<T> cbegin() const;
    ConstFrame<T> cend() const;
    unsigned channels() const;
    Sample<T> *data();
    const Sample<T> *data() const;
    Frame<T> end();
    unsigned frames() const;

    void fromArray(const void *array, Sound::Type type,
                   unsigned channels, unsigned frames);

    template<typename S>
    void fromArray(const Sample<S> *array,
                   unsigned channels, unsigned frames);

    template<typename S>
    void fromBuffer(const Buffer<S> &buffer,
                    unsigned offset = 0, unsigned count = 0);

    bool isEmpty() const;
    void mono(const Buffer<T> &source, bool mixdown = true);
    T *ptr();
    const T *ptr() const;
    void reallocate(unsigned channels, unsigned frames);
    Buffer<T> resample(unsigned destRate, unsigned sourceRate,
                       Sound::Quality quality = Sound::QualityBest);
    void resize(unsigned frames);
    void resample(const Buffer<T> &source,
                  unsigned destRate, unsigned sourceRate,
                  Sound::Quality quality = Sound::QualityBest);

    void peekChannels(const Buffer<T> &source,
                      unsigned count, bool mixdown = true);
    void silence();
    unsigned size() const;
    void stereo(const Buffer<T> &source, bool pullup = true);

private:
    unsigned _channels;
    unsigned _frames;
    std::vector<Sample<T>> _samples;
};

SOUND_INSTANTIATE_DECLARATION(Buffer)

#endif // BUFFER_H
