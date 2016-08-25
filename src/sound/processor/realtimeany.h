#ifndef SOUND_PROCESSOR_REALTIMEANY_H
#define SOUND_PROCESSOR_REALTIMEANY_H

#include <cassert>
#include <typeinfo>

namespace Sound {

class RealtimeAny {
public:
    RealtimeAny() {}
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
        return reinterpret_cast<T>(_value.vPointer);
    }

    template <typename T>
    void assign(T *pointer)
    {
        assert(typeid(T*) == *_type);
        _value.vPointer = pointer;
    }

    template <typename T>
    void assign(const T *pointer)
    {
        assert(typeid(const T*) == *_type);
        _value.vConstPointer = pointer;
    }

    void assign(RealtimeAny that)
    {
        assert(*_type == *that._type);
        _value = that._value;
    }

    template <typename T>
    void put(T value)
    {
        assert(typeid(T) == *_type);
        assign(value);
    }

    template <typename T>
    operator T() const
    {
        assert(typeid(T) == *_type);
        return as<T>();
    }

private:
    const std::type_info* _type;
    union Data {
        bool vBool;
        double vDouble;
        int vInt;
        void* vPointer;
        const void *vConstPointer;
        unsigned vUnsigned;
    } _value;

    template <typename T>
    void assign(T value);
};

}

#include "realtimeany.tcc"

#endif // SOUND_PROCESSOR_REALTIMEANY_H
