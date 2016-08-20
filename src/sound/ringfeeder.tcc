#include "ringfeeder.h"

namespace Sound {

template <typename T, typename D>
RingFeeder<T, D>::RingFeeder(RingBuffer<D>& ring)
    : _ring(ring)
{
}

template <typename T, typename D>
RingFeeder<T, D>::~RingFeeder()
{
    stop();
}

template <typename T, typename D>
void RingFeeder<T, D>::start(FeedFunc feed, unsigned usleep)
{
    stop();
    _loopThread = std::thread(&RingFeeder<T, D>::loop, this, feed, usleep);
}

template <typename T, typename D>
void RingFeeder<T, D>::stop()
{
    if (_loopThread.joinable()) {
        _isStopping = true;
        _loopThread.join();
    }
}

template <typename T, typename D>
void RingFeeder<T, D>::loop(FeedFunc feed, unsigned usleep)
{
    _isStopping = false;

    do {
        ConstFrame<T> beg, end;

        if (!_ring.isHalfFull()) {
            do {
                _isStopping = feed(beg, end);

                if (end - beg > 0) {
                    _ring.push(beg, end);
                }
            } while (!_isStopping && !_ring.isFull());
        }

        if (!_isStopping) {
            std::this_thread::sleep_for(std::chrono::milliseconds(usleep));
        }
    } while (!_isStopping);
}
}
