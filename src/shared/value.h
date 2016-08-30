#ifndef SHARED_VALUE_H
#define SHARED_VALUE_H

#include "log.h"
#include <type_traits>
#include <typeinfo>

/**
 * @brief The Value class
 *
 * Union like std::any but suitable for realtime object messaging
 *
 * Notes:
 *
 * 1. Own Null type, which can be accepted by any value to make it zero.
 * 2. Value type is set on creation or initial assignment, and can't be changed
 * later. Exception is for NullType, which means uninitialized.
 * 3. Strict typing.
 * 4. Only 32-bit values accepted, used float instead of double.
 */
class Value {
public:
    enum NullType : unsigned {
        Null = 0,
    };

    Value()
        : _data(Null)
        , _type(&typeid(NullType))
    {
    }

    Value(NullType)
        : Value()
    {
    }

    template <typename T>
    Value(T* p)
        : _data(const_cast<const T*>(p))
        , _type(&typeid(const T*))
    {
    }

    Value(std::nullptr_t)
        : _data(NULL)
        , _type(&typeid(const void*))
    {
    }

    template <typename T>
    Value(const T& t)
    {
        if (std::is_integral<T>::value) {
            if (typeid(bool) == typeid(T)) {
                _type = &typeid(bool);
            } else {
                _type = &typeid(int);
            }

            _data = Data(static_cast<int>(t));
        } else if (std::is_enum<T>::value) {
            _type = &typeid(T);
            _data = Data(static_cast<int>(t));
        } else if (std::is_floating_point<T>::value) {
            _type = &typeid(float);
            _data = Data(static_cast<float>(t));
        } else {
            BAD_CAST("Given value type is not supported");
        }
    }

    template <typename T>
    T as() const
    {
        bool isValidType = typeid(T) == type()
            || (std::is_integral<T>::value && type() == typeid(int))
            || (std::is_floating_point<T>::value && type() == typeid(float));

        if (!isValidType) {
            LOG(DEBUG, "Value type is "
                    << type().name()
                    << ", but attempting to access "
                    << typeid(T).name());
            BAD_CAST("Trying to get wrong type from value");
        }

        return *reinterpret_cast<const T*>(&_data);
    }

    const std::type_info& type() const
    {
        return *_type;
    }

    template <typename T>
    operator T() const
    {
        return as<T>();
    }

    Value& operator=(NullType)
    {
        _data.intVal = 0;
        return *this;
    }

    Value& operator=(const Value& that)
    {
        if (type() != that.type() && type() != typeid(NullType)) {
            BAD_CAST("Can't assign from another value without exact type match.");
        }

        _type = that._type;
        _data = that._data;
        return *this;
    }

    template <typename T>
    Value& operator=(const T* p)
    {
        if (typeid(NullType) == type()) {
            _data = Data(p);
            _type = &typeid(const T*);
        } else if (typeid(const T*) == type()) {
            _data.ptrVal = p;
        } else {
            BAD_CAST("Assigning value of wrong type");
        }

        return *this;
    }

private:
    union Data {
        Data() {}
        Data(NullType)
            : intVal(0)
        {
        }

        Data(const Data& that)
            : intVal(that.intVal)
        {
        }

        Data(float source)
            : floatVal(source)
        {
        }

        Data(int source)
            : intVal(source)
        {
        }

        Data(const void* source)
            : ptrVal(source)
        {
        }

        template <typename T>
        Data(const T* source)
            : ptrVal(reinterpret_cast<const void*>(source))
        {
        }

        template <typename T>
        Data(const T& source)
            : intVal(static_cast<int>(source))
        {
            static_assert(std::is_enum<T>::value,
                "Value type is not allowed");
        }

        float floatVal;
        int intVal;
        const void* ptrVal;
    } _data;

    const std::type_info* _type;
};

#include "value.tcc"

#endif // SHARED_VALUE_H
