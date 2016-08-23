#ifndef ANY_H
#define ANY_H

#include <cassert>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <utility>

#include "log.h"

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

    bool like(const Any& that)
    {
        return !_ptr || !that._ptr
            || _ptr->type_info() == that._ptr->type_info();
    }

    bool like(const Any&& that)
    {
        return !_ptr || !that._ptr
            || _ptr->type_info() == that._ptr->type_info();
    }

    template <typename V>
    StorageType<V>& as()
    {
        bool strict = true;

        if (strict) {
            if (_ptr->type_info() != typeid(V)) {
                LOG(ERROR, "Strict type is "
                        << _ptr->type_info().name()
                        << ", attempted to get: "
                        << typeid(V).name());
                LOGIC_ERROR("Trying to get value of wrong type");
            }
        }

        auto derived = dynamic_cast<Wrapper<StorageType<V> >*>(_ptr);

        if (!derived) {
            throw std::bad_cast();
        }

        return derived->value;
    }

    const std::type_info& type_info() const
    {
        if (!_ptr) {
            return typeid(nullptr);
        }

        return _ptr->type_info();
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
        virtual const std::type_info& type_info() const = 0;
    };

    template <typename T>
    struct Wrapper : WrapperBase {
        T value;

        template <typename V>
        Wrapper(V&& value)
            : value(std::forward<V>(value))

        {
        }

        WrapperBase* clone() const override
        {
            return new Wrapper<T>(value);
        }

        const std::type_info& type_info() const override
        {
            return typeid(T);
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
