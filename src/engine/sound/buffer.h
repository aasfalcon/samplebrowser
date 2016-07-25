#ifndef BUFFER_H
#define BUFFER_H

namespace Sound {

/**
 * Channel
 */
enum Channel {
    M = 0,
    L = M,
    R,
    FL = L,
    FR = R,
    FC,
    LF,
    BL,
    BR,
    FLC,
    FRC,
    BC,
    SL,
    SR,
    TC,
    TFL,
    TFC,
    TFR,
    TBL,
    TBC,
    TBR,
};

/**
 * Sample
 */
template<class T>
class Sample {
    Sample(T value): _value(value) {}

    operator T() const {
        return _value;
    }

    operator T&() {
        return _value;
    }

private:
    T _value;
};

/**
 * Frame
 */
template<class T>
class Frame {
public:
    typedef ChannelNumber Channel;

    Frame(Sample<T> *data, int channels)
        : _channels(channels)
        , _samples(data)
    {
    }

    T get(Channel channel = M) const {
        return _samples[channel];
    }

    void next() {
        _samples += _channels;
    }

    void set(T value, Channel channel = M) {
        _samples[channel] = value;
    }

private:
    int _channels;
    Sample<T> *_samples;
};

/**
 * Buffer
 */
template<class T>
class Buffer {
public:
    Buffer(int frames, int channels = 1)
        : _channels(channels)
        , _data(NULL)
        , _frames(0)
    {
        resize(length);
    }

     ~Buffer() {
        clear();
     }

     Buffer channel(Channel channel) {
        Buffer result(_frames);
        Frame<T> source = frame();
        Frame<T> dest = result.frame();

        for (int i = 0; i < _length; i++) {
            dest.set(source.get(channel));
            source.next();
            dest.next();
        }

        return result;
     }


     void clear() {
        resize(0);
     }

     Sample *data() {
         return _data;
     }

     Frame<T> frame() {
         return Frame<T>(_data, _channels);
     }

     int frames() const {
        return _frames;
     }

     void resize(int frames) {
        if (_frames != frames) {
            if (_data) {
                delete _data;
                _data = NULL;
            }

            _frames = frames;

            if (frames) {
                _data = new T[frames];
            }

        }
    }

private:
    Buffer(const Buffer &buffer)
        : _channels(buffer._channels)
        , _data(buffer._data)
        , _frames(buffer._frames)
    {}

    int _channels;
    T *_data;
    int _frames;
};

#endif // BUFFER_H
