#ifndef SOUND_PROCESSOR_REALTIMEANY_H
#define SOUND_PROCESSOR_REALTIMEANY_H

#include <cassert>
#include <typeinfo>

namespace Sound {

class RealtimeAny {
public:
    RealtimeAny()
        : _type(nullptr)
    {
    }

    RealtimeAny(const RealtimeAny& that)
        : _type(that._type)
        , _value(that._value)
    {
    }

    template <typename T>
    RealtimeAny(T value)
        : _type(&typeid(T))
    {
        assign(value);
    }

    template <typename T>
    T as() const
    {
        assert(typeid(void*) == *_type);
        return reinterpret_cast<T>(_value.vPtr);
    }

    template <typename T>
    operator T() const
    {
        assert(typeid(T) == *_type);
        return as<T>();
    }

    bool operator !() const
    {
        return !_value.vPtr;
    }

private:
    const std::type_info* _type;
    union Data {
        Data()
            : vPtr(nullptr)
        {
        }

        bool vBool;
        float vFloat;
        int vInt;
        void* vPtr;
        const void* vCPtr;
        unsigned vUInt;
    } _value;

    template <typename T>
    inline void assign(T value);

    template <typename T>
    void assign(T* pointer)
    {
        assert(typeid(T*) == *_type);
        _value.vPtr = pointer;
    }

    template <typename T>
    void assign(const T* pointer)
    {
        assert(typeid(const T*) == *_type);
        _value.vCPtr = pointer;
    }

    void assign(RealtimeAny that)
    {
        assert(*_type == *that._type);
        _value = that._value;
    }
};
}

#include "realtimeany.tcc"

#endif // SOUND_PROCESSOR_REALTIMEANY_H
