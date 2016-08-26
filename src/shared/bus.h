#ifndef SHARED_BUS_H
#define SHARED_BUS_H

#include <cassert>
#include "log.h"
#include "ring.h"

template <typename T>
class Bus : public Ring<T> {
public:
    Bus(unsigned ringSize, unsigned heldFactor = 10)
        : Ring<T>(ringSize)
        , _held(ringSize * heldFactor)
        , _lost(0)
    {
    }

    ~Bus() override {}

    void clear() override
    {
        Ring<T>::clear();
        _held.clear();
        _lost = 0;
    }

    void clearLost()
    {
        _lost = 0;
    }

    unsigned held() const
    {
        return _held.loaded();
    }

    void flush()
    {
        unsigned count = std::min(held(), this->space());

        while (count--) {
            this->push(_held.pop());
        }
    }

    unsigned lost() const
    {
        return _lost;
    }

    const T &pop() override
    {
        return Ring<T>::pop();
    }

    void push(const T &item) override
    {
        if (this->space()) {
            Ring<T>::push(item);
        } else if (_held.space()) {
            _held.push(item);
        } else {
            ++_lost;
#ifndef NDEBUG
            LOG_IF(1 == _lost, ERROR, "Bus held buffer overflow");
#endif
        }
    }

private:
    Ring<T> _held;
    unsigned _lost;
};

#endif // SHARED_BUS_H
