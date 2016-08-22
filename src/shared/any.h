#ifndef ANY_H
#define ANY_H

#include <cassert>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

template <class T>
using StorageType = typename std::decay<T>::type;

struct Any {
    bool is_null() const { return !_ptr; }

    Any()
        : _ptr(nullptr)
    {
    }

    Any(Any& that)
        : _ptr(that.clone())
    {
    }

    Any(Any&& that)
        : _ptr(that._ptr)
    {
        that._ptr = nullptr;
    }

    Any(const Any& that)
        : _ptr(that.clone())
    {
    }

    Any(const Any&& that)
        : _ptr(that.clone())
    {
    }

    template <typename V>
    Any(V&& value)
        : _ptr(new Wrapper<StorageType<V> >(std::forward<V>(value)))
    {
    }

    template <typename V>
    bool is() const
    {
        return dynamic_cast<Wrapper<StorageType<V> >*>(_ptr);
    }

    template <typename V>
    StorageType<V>& as()
    {
        auto derived = dynamic_cast<Wrapper<StorageType<V> >*>(_ptr);

        if (!derived) {
            throw std::bad_cast();
        }

        return derived->value;
    }

    template <typename V>
    operator V()
    {
        return as<StorageType<V> >();
    }

    Any& operator=(const Any& a)
    {
        if (_ptr != a._ptr) {
            auto old_ptr = _ptr;
            _ptr = a.clone();
            delete old_ptr;
        }

        return *this;
    }

    Any& operator=(Any&& a)
    {
        if (_ptr != a._ptr) {
            std::swap(_ptr, a._ptr);
        }

        return *this;
    }

    ~Any()
    {
        delete _ptr;
    }

private:
    struct WrapperBase {
        virtual ~WrapperBase() {}

        virtual WrapperBase* clone() const = 0;
    };

    template <typename T>
    struct Wrapper : WrapperBase {
        template <typename V>
        Wrapper(V&& value)
            : value(std::forward<V>(value))
        {
        }

        T value;

        WrapperBase* clone() const
        {
            return new Wrapper<T>(value);
        }
    };

    WrapperBase* clone() const
    {
        if (!_ptr) {
            return nullptr;
        }

        return _ptr->clone();
    }

    WrapperBase* _ptr;
};

#endif // ANY_H
