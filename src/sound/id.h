#ifndef SOUND_ID_H
#define SOUND_ID_H

#include <cassert>

namespace Sound {

struct Index;

struct ID {
    ID() {}

    template <typename E>
    ID(E value)
        : _value(static_cast<unsigned>(value))
    {
        static_assert(E::_Begin != E::_End, "Wrong ID enum template type");
    }

    template <typename T>
    T as() const
    {
        return static_cast<T>(_value);
    }

    template <typename E>
    bool is() const
    {
        return E::_Begin < _value && E::_End > _value;
    }

    template <typename E>
    operator E() const
    {
        static_assert(E::_Begin != E::_End, "Wrong ID enum template type");
        assert(is<E>());
        return static_cast<E>(_value);
    }

    bool operator==(const ID& that) const { return this->_value == that._value; }
    bool operator!=(const ID& that) const { return this->_value != that._value; }
    bool operator<=(const ID& that) const { return this->_value <= that._value; }
    bool operator>=(const ID& that) const { return this->_value >= that._value; }
    bool operator<(const ID& that) const { return this->_value < that._value; }
    bool operator>(const ID& that) const { return this->_value > that._value; }

private:
    friend struct Index;
    unsigned _value;
};

struct Index : public ID {
    Index(const ID &id)
    {
        this->_value = id._value;
    }
};

}

#endif // SOUND_ID_H
