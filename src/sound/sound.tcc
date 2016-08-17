#include "sound.h"

template<typename T>
Sound::Object<T>::~Object()
{

}

template<typename T>
struct always_false {
    enum { value = false };
};

template<typename T>
Sound::Type Sound::Object<T>::type()
{
    static_assert(always_false<T>::value, "Only Sound::TypeXXX types are supported");
}

namespace Sound {
    template<> Type Object<Float32>::type() { return TypeFloat32; }
    template<> Type Object<Float64>::type() { return TypeFloat64; }
    template<> Type Object<Int8>::type() { return TypeInt8; }
    template<> Type Object<Int16>::type() { return TypeInt16; }
    template<> Type Object<Int24>::type() { return TypeInt24; }
    template<> Type Object<Int32>::type() { return TypeInt32; }
}
