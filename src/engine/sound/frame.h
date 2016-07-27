#ifndef FRAME
#define FRAME

namespace Sound {

template<class T>
class ReadFrame {
public:
    inline ReadFrame()
        : _channels(0)
        , _samples(0)
    {}
    inline ReadFrame(const T *data, int channels)
        : _channels(channels)
        , _samples(data)
    {
    }

    inline int channels() const {
        return _channels;
    }

    inline T get(int channel) const {
        return _samples[channel];
    }

    inline void next() {
        _samples += _channels;
    }

protected:
    int _channels;
    const T *_samples;
};

template<class T>
class Frame : public ReadFrame<T> {
public:
    Frame(T *data, int channels)
        : ReadFrame<T>(data, channels)
    {
    }

    void set(T value, int channel = 0) {
        *((T *)this->_samples + channel) = value;
    }
};

} // namespace Sound

#endif // FRAME

