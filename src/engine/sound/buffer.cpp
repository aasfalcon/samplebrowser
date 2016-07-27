#include <samplerate.h>
#include <cstring>
#include <samplerate.h>

#include "buffer.h"
#include "frame.h"

using namespace Sound;

template<class T>
Buffer<T>::Buffer(int frames, int channels)
    : _channels(channels)
    , _frames(0)
{
    if (frames) {
        framesResize(frames);
    }
}

template<class T>
Buffer<T>::Buffer(const Buffer<T> &buffer)
    : QVector<T>::QVector(buffer)
    , _channels(buffer._channels)
    , _frames(buffer._frames)
{
}

template<class T>
Buffer<T> Buffer<T>::channel(int channel) const {
    Buffer<T> result(_frames);
    ReadFrame<T> source = frame();
    Frame<T> dest = result.frame();

    for (int i = 0; i < _frames; i++) {
        dest.set(source.get(channel));
        source.next();
        dest.next();
    }

    return result;
}

template<class T>
int Buffer<T>::channels() const {
    return _channels;
}

template<class T>
Frame<T> Buffer<T>::frame(int position) {
    return Frame<T>(this->data() + position * _channels, _channels);
}

template<class T>
ReadFrame<T> Buffer<T>::frame(int position) const {
    return ReadFrame<T>(this->data() + position * _channels, _channels);
}

template<class T>
int Buffer<T>::frames() const {
    return _frames;
}

template<class T>
Buffer<T> Buffer<T>::stereo() const {
    if (_channels == 2) {
        return *this;
    }

    Buffer<T> result(_frames, 2);
    ReadFrame<T> source = frame();
    Frame<T> dest = result.frame();
    bool hasSecond = _channels > 2;

    for (int i = 0; i < _frames; i++) {
        dest.set(source.get(0), 0);
        dest.set(source.get((int)hasSecond), 1);
        dest.next();
        source.next();
    }

    return result;
}

template<class T>
void Buffer<T>::framesResize(int frames, int channels) {
    if (channels <= 0) {
        channels = _channels;
    }

    if (_frames != frames || _channels != channels) {
        this->resize(frames * channels);
        _frames = frames;
        _channels = channels;
    }
}

template<class T>
Buffer<T> Buffer<T>::mono() const
{
    if (_channels == 1) {
        return *this;
    }

    Buffer<T> result(_frames);
    ReadFrame<T> source = frame();
    Frame<T> dest = result.frame();
    int channels = source.channels();

    for (int i = 0; i < _frames; i++) {
        double sample = .0;

        for (int j = 0; j < channels; j++) {
            sample += source.get(j);
        }

        dest.set(sample / channels);
        source.next();
        dest.next();
    }

    return result;
}

template class Buffer<short>;
template class Buffer<int>;
template class Buffer<float>;
template class Buffer<double>;
