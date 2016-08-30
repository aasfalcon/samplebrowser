#ifndef SHARED_BUS_H
#define SHARED_BUS_H

#include <cassert>
#include "log.h"
#include "ring.h"

template <typename T>
class Bus : public Ring<T> {
public:
    Bus(unsigned ringSize, unsigned holdFactor = 10)
        : Ring<T>(ringSize)
        , _held(ringSize * holdFactor)
        , _lost(0)
    {
    }

    ~Bus() override {}

    void clear() override
    {
        Ring<T>::clear();
        _held.clear();
        clearLost();
    }

    void clearLost()
    {
        _lost = 0;
    }

    unsigned held() const
    {
        return _held.loaded();
    }

    void load()
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

    void push(const T &item) override
    {
        if (this->space()) {
            Ring<T>::push(item);
        } else if (_held.space()) {
            _held.push(item);
        } else {
            ++_lost;
            OVERFLOW_ERROR("Bus buffer overflow");
        }
    }

private:
    Ring<T> _held;
    unsigned _lost;
};

#endif // SHARED_BUS_H
