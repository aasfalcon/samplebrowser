#include "generator_test.h"

#include "frame.h"
#include "constframe.h"

class SoundFrameTest : public GeneratorTest {
public:
    SoundFrameTest() {
        for (unsigned i = 0; i < TEST_CHANNELS; i++) {
            pr[i].value() = random();
            i8[i] = pr[i];
            i16[i] = pr[i];
            i24E[i] = pr[i];
            i32[i] = pr[i];
            f32[i] = pr[i];
            f64[i] = pr[i];
        }
    }

    void clearTest() {
        for (unsigned i =0; i < TEST_CHANNELS; i++) {
            test_i8[i].silence();
            test_i16[i].silence();
            test_i24E[i].silence();
            test_i32[i].silence();
            test_f32[i].silence();
            test_f64[i].silence();
            test_pr[i].silence();
        }
    }

    constexpr static unsigned TEST_CHANNELS = 64;

    Sample<Int8> i8[TEST_CHANNELS];
    Sample<Int16> i16[TEST_CHANNELS];
    Sample<Int24E> i24E[TEST_CHANNELS];
    Sample<Int32> i32[TEST_CHANNELS];
    Sample<Float32> f32[TEST_CHANNELS];
    Sample<Float64> f64[TEST_CHANNELS];
    Sample<Precise> pr[TEST_CHANNELS];

    Sample<Int8> test_i8[TEST_CHANNELS];
    Sample<Int16> test_i16[TEST_CHANNELS];
    Sample<Int24E> test_i24E[TEST_CHANNELS];
    Sample<Int32> test_i32[TEST_CHANNELS];
    Sample<Float32> test_f32[TEST_CHANNELS];
    Sample<Float64> test_f64[TEST_CHANNELS];
    Sample<Precise> test_pr[TEST_CHANNELS];
};

using namespace Sound;

TEST_F(SoundFrameTest, ctor)
{
    NO_THROW(Frame<Int8> f);
    NO_THROW(Frame<Int16> f);
    NO_THROW(Frame<Int24E> f);
    NO_THROW(Frame<Int32> f);
    NO_THROW(Frame<Float32> f);
    NO_THROW(Frame<Float64> f);

    NO_THROW(Frame<Precise> f);
    NO_THROW(Frame<Int> f);
    NO_THROW(Frame<Float> f);

    Int8 i8v[TEST_CHANNELS];
    Int16 i16v[TEST_CHANNELS];
    Int24E i24Ev[TEST_CHANNELS];
    Int32 i32v[TEST_CHANNELS];
    Float32 f32v[TEST_CHANNELS];
    Float64 f64v[TEST_CHANNELS];
    Precise prv[TEST_CHANNELS];

    for (unsigned i = 1; i <= TEST_CHANNELS; i++) {
        ASSERT_NO_THROW(Frame<Int8>(i, i8));
        ASSERT_NO_THROW(Frame<Int16>(i, i16));
        ASSERT_NO_THROW(Frame<Int24E>(i, i24E));
        ASSERT_NO_THROW(Frame<Int32>(i, i32));
        ASSERT_NO_THROW(Frame<Float32>(i, f32));
        ASSERT_NO_THROW(Frame<Float64>(i, f64));
        ASSERT_NO_THROW(Frame<Precise>(i, pr));

        ASSERT_NO_THROW(Frame<Int8>(i, i8v));
        ASSERT_NO_THROW(Frame<Int16>(i, i16v));
        ASSERT_NO_THROW(Frame<Int24E>(i, i24Ev));
        ASSERT_NO_THROW(Frame<Int32>(i, i32v));
        ASSERT_NO_THROW(Frame<Float32>(i, f32v));
        ASSERT_NO_THROW(Frame<Float64>(i, f64v));
        ASSERT_NO_THROW(Frame<Precise>(i, prv));
    }
}

TEST_F(SoundFrameTest, data)
{
    clearTest();

    Frame<Int8> fi8(TEST_CHANNELS, test_i8);
    Frame<Int16> fi16(TEST_CHANNELS, test_i16);
    Frame<Int24E> fi24E(TEST_CHANNELS, test_i24E);
    Frame<Int32> fi32(TEST_CHANNELS, test_i32);
    Frame<Float32> ff32(TEST_CHANNELS, test_f32);
    Frame<Float64> ff64(TEST_CHANNELS, test_f64);
    Frame<Precise> fpr(TEST_CHANNELS, test_pr);

    fpr.put(Frame<Precise>(TEST_CHANNELS, pr));
    fi8.put(fpr);
    fi16.put(fpr);
    fi24E.put(fpr);
    fi32.put(fpr);
    ff32.put(fpr);
    ff64.put(fpr);

    for (unsigned i = 0; i < TEST_CHANNELS; i++) {
        EQ(fpr[i].value(), pr[i].value());
        NEAR(fi8[i].value(), i8[i].value(), LSB_ERROR * 2);
        NEAR(fi16[i].value(), i16[i].value(), LSB_ERROR * 2);
        NEAR(fi24E[i].value(), i24E[i].value(), LSB_ERROR * 2);
        NEAR(fi32[i].value(), i32[i].value(), LSB_ERROR * 2);
        EQ(ff32[i].value(), f32[i].value());
        EQ(ff64[i].value(), f64[i].value());
    }
}
