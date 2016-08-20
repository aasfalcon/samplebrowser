#ifndef RINGFEEDER_H
#define RINGFEEDER_H

#include "ringbuffer.h"
#include <functional>
#include <thread>

namespace Sound {

template <typename T, typename D>
class RingFeeder : public Object<T> {
public:
    typedef std::function<bool (ConstFrame<T>&, ConstFrame<T>&)>
        FeedFunc;

    RingFeeder(RingBuffer<D>& ring);
    ~RingFeeder();

    void start(FeedFunc feed, unsigned usleep);
    void stop();

private:
    std::thread _loopThread;
    RingBuffer<D>& _ring;
    bool _isStopping;

    void loop(FeedFunc feed, unsigned usleep);
};
}

#include "ringfeeder.tcc"

#endif // RINGFEEDER_H
