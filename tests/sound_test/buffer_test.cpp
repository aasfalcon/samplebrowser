#include "generator_test.h"

#include "buffer.h"

class SoundBufferTest : public GeneratorTest {
};

TEST_F(SoundBufferTest, ctor)
{
    NO_THROW(Buffer<Int8> b);
    NO_THROW(Buffer<Int16> b);
    NO_THROW(Buffer<Int24E> b);
    NO_THROW(Buffer<Int32> b);
    NO_THROW(Buffer<Int> b);
    NO_THROW(Buffer<Float32> b);
    NO_THROW(Buffer<Float64> b);
    NO_THROW(Buffer<Float> b);
    NO_THROW(Buffer<Precise> b);

    ANY_THROW(Buffer<Int8> b(0, 0));
    ANY_THROW(Buffer<Int16> b(0, 0));
    ANY_THROW(Buffer<Int24E> b(0, 0));
    ANY_THROW(Buffer<Int32> b(0, 0));
    ANY_THROW(Buffer<Int> b(0, 0));
    ANY_THROW(Buffer<Float32> b(0, 0));
    ANY_THROW(Buffer<Float64> b(0, 0));
    ANY_THROW(Buffer<Float> b(0, 0));
    ANY_THROW(Buffer<Precise> b(0, 0));

    ANY_THROW(Buffer<Int8> b(0, 1));
    ANY_THROW(Buffer<Int16> b(0, 1));
    ANY_THROW(Buffer<Int24E> b(0, 1));
    ANY_THROW(Buffer<Int32> b(0, 1));
    ANY_THROW(Buffer<Int> b(0, 1));
    ANY_THROW(Buffer<Float32> b(0, 1));
    ANY_THROW(Buffer<Float64> b(0, 1));
    ANY_THROW(Buffer<Float> b(0, 1));
    ANY_THROW(Buffer<Precise> b(0, 1));

    ANY_THROW(Buffer<Int8> b(1, 0));
    ANY_THROW(Buffer<Int16> b(1, 0));
    ANY_THROW(Buffer<Int24E> b(1, 0));
    ANY_THROW(Buffer<Int32> b(1, 0));
    ANY_THROW(Buffer<Int> b(1, 0));
    ANY_THROW(Buffer<Float32> b(1, 0));
    ANY_THROW(Buffer<Float64> b(1, 0));
    ANY_THROW(Buffer<Float> b(1, 0));
    ANY_THROW(Buffer<Precise> b(1, 0));

    NO_THROW(Buffer<Int>(1, 1));
    NO_THROW(Buffer<Float>(1, 1));
    NO_THROW(Buffer<Precise>(1, 1));
    NO_THROW(Buffer<Int8>(1, 1));
    NO_THROW(Buffer<Int16>(1, 1));
    NO_THROW(Buffer<Int24E>(1, 1));
    NO_THROW(Buffer<Int32>(1, 1));
    NO_THROW(Buffer<Float32>(1, 1));
    NO_THROW(Buffer<Float64>(1, 1));

    // selective
    NO_THROW(Buffer<Float32> b(1, 1));
    NO_THROW(Buffer<Float64> b(2, 10));
    NO_THROW(Buffer<Int8> b(3, 100));
    NO_THROW(Buffer<Int16> b(4, 1000));
    NO_THROW(Buffer<Int24E> b(4, 1000));
    NO_THROW(Buffer<Int32> b(5, 10000));

    NO_THROW(Buffer<Float32> b(1, 1));
    NO_THROW(Buffer<Float64> b(10, 2));
    NO_THROW(Buffer<Int8> b(100, 3));
    NO_THROW(Buffer<Int16> b(1000, 4));
    NO_THROW(Buffer<Int24E> b(1000, 4));
    NO_THROW(Buffer<Int32> b(10000, 5));

    NO_THROW(Buffer<Int> b(6, 100000));
    NO_THROW(Buffer<Float> b(7, 1000000));
    NO_THROW(Buffer<Precise> b(8, 10000000));
    NO_THROW(Buffer<Precise> b(10000000, 8));

    // copy
    unsigned maxChannels = 16;
    unsigned maxFrames = 16;

    for (unsigned c = 1; c <= maxChannels; c++) {
        for (unsigned f = 1; f <= maxFrames; f++) {
            ASSERT_NO_THROW(Buffer<Precise> p(c, f));
            Buffer<Precise> reference(c, f);

            for (unsigned i = 0; i < f; i++) {
                for (unsigned j = 0; j < c; j++) {
                    reference.data()[i * c + j] = random();
                }
            }

            ASSERT_NO_THROW(Buffer<Int8> b = reference);
            ASSERT_NO_THROW(Buffer<Int16> b = reference);
            ASSERT_NO_THROW(Buffer<Int24E> b = reference);
            ASSERT_NO_THROW(Buffer<Int32> b = reference);
            ASSERT_NO_THROW(Buffer<Float32> b = reference);
            ASSERT_NO_THROW(Buffer<Float64> b = reference);

            Buffer<Int8> i8 = reference;
            Buffer<Int16> i16 = i8;
            Buffer<Int24E> i24E = i8;
            Buffer<Int32> i32 = i8;
            Buffer<Float32> f32 = i8;
            Buffer<Float64> f64 = i8;

            ASSERT_EQ(i8.channels(), c);
            ASSERT_EQ(i8.frames(), f);
            ASSERT_EQ(i16.channels(), c);
            ASSERT_EQ(i16.frames(), f);
            ASSERT_EQ(i24E.channels(), c);
            ASSERT_EQ(i24E.frames(), f);
            ASSERT_EQ(i32.channels(), c);
            ASSERT_EQ(i32.frames(), f);
            ASSERT_EQ(f32.channels(), c);
            ASSERT_EQ(f32.frames(), f);
            ASSERT_EQ(f64.channels(), c);
            ASSERT_EQ(f64.frames(), f);

            for (unsigned i = 0; i < f; i++) {
                for (unsigned j = 0; j < c; j++) {
                    Int8 i8v = i8.data()[i * c + j].value();
                    Sample<Int8> i8s(i8v);
                    ASSERT_EQ((i8.cbegin() + int(i))[j].value(), Sample<Int8>(i8s).value());
                    ASSERT_EQ((i16.cbegin() + int(i))[j].value(), Sample<Int16>(i8s).value());
                    ASSERT_EQ((i24E.cbegin() + int(i))[j].value(), Sample<Int24E>(i8s).value());
                    ASSERT_EQ((i32.cbegin() + int(i))[j].value(), Sample<Int32>(i8s).value());
                    ASSERT_EQ((f32.cbegin() + int(i))[j].value(), Sample<Float32>(i8s).value());
                    ASSERT_EQ((f64.cbegin() + int(i))[j].value(), Sample<Float64>(i8s).value());
                }
            }
        }
    }

    // copy selective
    Buffer<Int> ib(2, 10);
    NO_THROW(Buffer<Float> fb(ib));
    Buffer<Float> fb(ib);
    EQ(fb.frames(), 10);
    EQ(fb.channels(), 2);

    Buffer<Float> fb2(6, 2000);
    Buffer<Int> ib2(fb2);
    EQ(ib2.frames(), 2000);
    EQ(ib2.channels(), 6);

    NO_THROW(Buffer<Float32> b(fb2.cbegin(), fb2.cend()));
    Buffer<Float32> fb3(fb2.cbegin(), fb2.cend());
    EQ(fb3.channels(), fb2.channels());
    EQ(fb3.frames(), fb2.frames());

    Buffer<Int8> ib3(fb3.cbegin(), fb3.cend());
    EQ(ib3.channels(), fb2.channels());
    EQ(ib3.frames(), fb2.frames());

    Buffer<Int16> a;
    Buffer<Float64> b(10, 20);
    a = b;
    EQ(a.channels(), b.channels());
    EQ(a.frames(), b.frames());
}

TEST_F(SoundBufferTest, iterators)
{
    Float* reference = new Float[1024 * 2];

    for (int i = 0; i < 1024 * 2; i++) {
        reference[i] = Float(random());
    }

    Buffer<Float> b(2, 1024, reference);
    EQ(b.cend() - b.cbegin(), 1024);
    EQ(b.end() - b.begin(), 1024);
    int f = 0;

    for (auto it = b.cbegin(); it != b.cend(); ++it) {
        ASSERT_EQ(it[0].value(), reference[f * 2 + 0]);
        ASSERT_EQ(it[1].value(), reference[f * 2 + 1]);
        ++f;
    }

    Buffer<Int8> b2(2, 1024);
    f = 0;

    for (auto it = b2.begin(); it != b2.end(); ++it) {
        it[0] = Sample<Float>(reference[f * 2 + 0]);
        it[1] = Sample<Float>(reference[f * 2 + 1]);
        ++f;
    }

    f = 0;

    for (auto it = b2.cbegin(); it != b2.cend(); ++it) {
        Sample<Precise> r1 = reference[f * 2 + 0];
        Sample<Precise> r2 = reference[f * 2 + 1];
        ASSERT_NEAR(it[0].value(), Sample<Int8>(r1).value(), LSB_ERROR);
        ASSERT_NEAR(it[1].value(), Sample<Int8>(r2).value(), LSB_ERROR);
        ++f;
    }

    delete[] reference;
}

TEST_F(SoundBufferTest, channelMixer)
{
    Float* reference = new Float[1024 * 2];

    for (int i = 0; i < 1024 * 2; i++) {
        reference[i] = Float(random());
    }

    // mixdown to mono
    Buffer<Float> stereo(2, 1024, reference);
    Buffer<Float> mono(1, 1024);
    mono.copyFrom(stereo.cbegin(), stereo.cend());

    int i = 0;

    for (auto mit = mono.cbegin(); mit != mono.cend(); ++mit) {
        Float computed = Float((Precise(reference[i]) + Precise(reference[i + 1])) / 2.0L);
        ASSERT_EQ(mit[0].value(), computed);
        i += 2;
    }
}
