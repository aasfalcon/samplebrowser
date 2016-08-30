#ifndef ID_H
#define ID_H

#include <cassert>

struct Index;

struct ID {
    ID()
        : _value(0)
    {
    }

    template <typename E>
    ID(E value)
        : _value(static_cast<unsigned>(value))
    {
        static_assert(E::_Begin != E::_End, "Wrong ID enum template type");
    }

    template <typename E>
    bool is() const
    {
        return E::_Begin < _value && E::_End > _value;
    }

    unsigned toUInt() const
    {
        return static_cast<unsigned>(_value);
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
    Index(const ID& id)
    {
        this->_value = id._value;
    }
};

#define ID_ENUM_NAMESPACE(a_ns)      \
    namespace a_ns {                 \
        enum class Base : unsigned { \
            _Begin,                  \
            _End = _Begin,           \
        };                           \
        typedef ::ID ID;             \
        typedef ::Index Index;       \
    }

#define ID_ENUM(a_ns, a_scope, a_base, ...)  \
    namespace a_ns {                         \
        enum class a_scope : unsigned {      \
            _Begin = unsigned(a_base::_End), \
            __VA_ARGS__,                     \
            _End,                            \
        };                                   \
    }

#endif // ID_H
