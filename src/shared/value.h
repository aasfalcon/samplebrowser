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
 * 4. Only 32-bit values accepted, so use float instead of double.
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

    Value(const Value& that)
        : _data(that._data)
        , _type(that._type)
    {
    }

    template <typename T>
    Value(T* p)
        : _data(const_cast<const T*>(p))
        , _type(&typeid(const T*))
    {
    }

    template <typename T>
    Value(const T& t)
        : _data(t)
        , _type(&typeid(T))
    {
    }

    template <typename T>
    T as() const
    {
        if (typeid(T) != type()) {
            LOG(DEBUG, "Value type is "
                    << type().name()
                    << ", but attempting to access "
                    << typeid(T).name());
            BAD_CAST("Trying to get wrong type from value");
        }

        return *((const T*)&_data.pv);
    }

    const std::type_info& type() const
    {
        return *_type;
    }

    Value& operator=(NullType)
    {
        _data.uv = 0;
        return *this;
    }

    template <typename T>
    operator T() const
    {
        return as<T>();
    }

    Value& operator=(const Value& v)
    {
        if (typeid(NullType) != type() && v.type() != type()) {
            BAD_CAST("Assigning value with wrong type");
        }

        _type = v._type;
        _data = v._data;
        return *this;
    }

    template <typename T>
    Value& operator=(const T* p)
    {
        if (typeid(NullType) == type()) {
            _data = Data(p);
            _type = &typeid(const T*);
        } else if (typeid(const void*) == type() || typeid(const T*) == type()) {
            _data.pv = p;
        } else {
            BAD_CAST("Assigning value with wrong type");
        }

        return *this;
    }

private:
    union Data {
        Data()
            : uv(0)
        {
        }

        Data(NullType)
            : uv(0)
        {
        }

        Data(const Data& that)
            : uv(that.uv)
        {
        }

        Data(bool source)
            : bv(source)
        {
        }

        Data(float source)
            : fv(source)
        {
        }

        Data(int source)
            : iv(source)
        {
        }

        Data(const void* source)
            : pv(source)
        {
        }

        Data(unsigned source)
            : uv(source)
        {
        }

        template <typename T>
        Data(const T* source)
        {
            pv = reinterpret_cast<const void*>(source);
        }

        template <typename T>
        Data(const T& source)
        {
            static_assert(std::is_enum<T>::value,
                "Given value type is not allowed");
            uv = unsigned(source);
        }

        bool bv;
        float fv;
        int iv;
        const void* pv;
        unsigned uv;
    } _data;

    const std::type_info* _type;
};

#include "value.tcc"

#endif // SHARED_VALUE_H
