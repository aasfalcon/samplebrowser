#ifndef BUFFER_H
#define BUFFER_H

#include <vector>

namespace Sound {

template<class T> class Frame;
template<class T> class ReadFrame;

template<class T>
class Buffer : public std::vector<T> {
public:
    Buffer(int frames = 0, int channels = 1);
    Buffer(const Buffer &source);

    Buffer channel(int channel) const;
    int channels() const;
    Frame<T> frame(int position = 0);
    ReadFrame<T> frame(int position = 0) const;
    int frames() const;
    void framesResize(int frames, int channels = 0);
    Buffer mono() const;
    Buffer stereo() const;

private:
    int _channels;
    int _frames;
};

} // namespace Sound

#endif // BUFFER_H
