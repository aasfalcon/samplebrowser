#include "common_test.h"

#include "buffer.h"

using namespace Sound;

TEST(SoundBufferTest, ctor)
{
    ANY_THROW(Buffer<Int>(0, 0));
    ANY_THROW(Buffer<Int>(0, 1));
    ANY_THROW(Buffer<Int>(1, 0));
    NO_THROW(Buffer<Int>(1, 1));


    NO_THROW(Buffer<Float32> b(1, 1));
    NO_THROW(Buffer<Float64> b(2, 10));
    NO_THROW(Buffer<Int8> b(3, 100));
    NO_THROW(Buffer<Int16> b(4, 1000));
    NO_THROW(Buffer<Int32> b(5, 10000));

    NO_THROW(Buffer<Int> b(6, 100000));
    NO_THROW(Buffer<Float> b(7, 1000000));
    NO_THROW(Buffer<Precise> b(8, 10000000));
}
