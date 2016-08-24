#ifndef SHARED_RING_H
#define SHARED_RING_H

#include <vector>

template <typename T>
class Ring {
public:
    Ring(unsigned count)
        : _begin(0)
        , _count(count)
        , _end(0)
        , _items(count)
    {
    }

    Ring(unsigned count, const T& value)
        : _begin(0)
        , _count(count)
        , _end(0)
        , _items(count, value)
    {
    }

    ~Ring() {}

    void clear()
    {
        _begin = _end;
    }

    unsigned count() const
    {
        return _count;
    }

    bool isEmpty() const
    {
        return loaded() == 0;
    }

    bool isFull() const
    {
        return loaded() == _count - 1;
    }

    bool isHalfFull() const
    {
        return loaded() >= _count / 2;
    }

    unsigned loaded() const
    {
        return (_count - _begin + _end) % _count;
    }

    unsigned space() const
    {
        return _count - 1 - loaded();
    }

    virtual const T& pop()
    {
        T& result = _items[_begin];

        ++_begin %= _count;
        return result;
    }

    virtual void push(const T& value)
    {
        _items[_end] = value;
        ++_end %= _count;
    }

protected:
    unsigned _begin;
    unsigned _count;
    unsigned _end;
    std::vector<T> _items;
};

#endif // SHARED_RING_H
