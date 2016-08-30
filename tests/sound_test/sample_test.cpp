#include "common_test.h"

#include "sample.h"

using namespace Sound;

const int LSB_ERROR = 2;
const int CYCLES = 16;

TEST(SoundSampleTest, limits)
{
    EQ(MAX_FLOAT32, 1.0f);
    EQ(MAX_FLOAT64, 1.0);
    EQ(MAX_INT8, INT8_MAX);
    EQ(MAX_INT16, INT16_MAX);
    EQ(MAX_INT32, INT32_MAX);

    EQ(MIN_FLOAT32, -1.0f);
    EQ(MIN_FLOAT64, -1.0);
    EQ(MIN_INT8, INT8_MIN);
    EQ(MIN_INT16, INT16_MIN);
    EQ(MIN_INT32, INT32_MIN);

    EQ(NIL_FLOAT32, 0.0f);
    EQ(NIL_FLOAT64, 0.0);
    EQ(NIL_INT8, 0);
    EQ(NIL_INT16, 0);
    EQ(NIL_INT32, 0);

    EQ(sizeof(Int8), 1);
    EQ(sizeof(Int16), 2);
    EQ(sizeof(Int32), 4);
    EQ(sizeof(Float32), 4);
    EQ(sizeof(Float64), 8);
    GE(sizeof(Precise), sizeof(Float64));

    EQ(typeid(Int24E), typeid(Float32));
}

TEST(SoundSampleTest, ctor)
{
    NO_THROW(Sample<Float32> s);
    NO_THROW(Sample<Float64> s);
    NO_THROW(Sample<Int8> s);
    NO_THROW(Sample<Int16> s);
    NO_THROW(Sample<Int32> s);

    NO_THROW(Sample<Float32> s(MAX_FLOAT32));
    NO_THROW(Sample<Float64> s(MAX_FLOAT64));
    NO_THROW(Sample<Int8> s(MAX_INT8));
    NO_THROW(Sample<Int16> s(MAX_INT16));
    NO_THROW(Sample<Int32> s(MAX_INT32));

    NO_THROW(Sample<Float32> s(MIN_FLOAT32));
    NO_THROW(Sample<Float64> s(MIN_FLOAT64));
    NO_THROW(Sample<Int8> s(MIN_INT8));
    NO_THROW(Sample<Int16> s(MIN_INT16));
    NO_THROW(Sample<Int32> s(MIN_INT32));
}

TEST(SoundSampleTest, ctorValue)
{
    NO_THROW(Sample<Float32> f32Max(MAX_FLOAT32));
    NO_THROW(Sample<Float32> f32Min(MIN_FLOAT32));
    NO_THROW(Sample<Float32> f32Nil(NIL_FLOAT32));

    NO_THROW(Sample<Float64> f64Max(MAX_FLOAT64));
    NO_THROW(Sample<Float64> f64Min(MIN_FLOAT64));
    NO_THROW(Sample<Float64> f64Nil(NIL_FLOAT64));

    NO_THROW(Sample<Int8> i8Max(MAX_INT8));
    NO_THROW(Sample<Int8> i8Min(MIN_INT8));
    NO_THROW(Sample<Int8> i8Nil(NIL_INT8));

    NO_THROW(Sample<Int16> i16Max(MAX_INT16));
    NO_THROW(Sample<Int16> i16Min(MIN_INT16));
    NO_THROW(Sample<Int16> i16Nil(NIL_INT16));

    NO_THROW(Sample<Int32> i32Max(MAX_INT32));
    NO_THROW(Sample<Int32> i32Min(MIN_INT32));
    NO_THROW(Sample<Int32> i32Nil(NIL_INT32));
}

TEST(SoundSampleTest, ctorCopy)
{
    Sample<Float32> f32Max(MAX_FLOAT32);
    Sample<Float32> f32Min(MIN_FLOAT32);
    Sample<Float32> f32Nil(NIL_FLOAT32);
    Sample<Float32> f32s1(f32Max);
    Sample<Float32> f32s2(f32Min);
    Sample<Float32> f32s3(f32Nil);
    EQ(Float32(f32s1), MAX_FLOAT32);
    EQ(Float32(f32s2), MIN_FLOAT32);
    EQ(Float32(f32s3), NIL_FLOAT32);

    Sample<Float64> f64Max(MAX_FLOAT64);
    Sample<Float64> f64Min(MIN_FLOAT64);
    Sample<Float64> f64Nil(NIL_FLOAT64);
    Sample<Float64> f64s1(f64Max);
    Sample<Float64> f64s2(f64Min);
    Sample<Float64> f64s3(f64Nil);
    EQ(Float64(f64s1), MAX_FLOAT64);
    EQ(Float64(f64s2), MIN_FLOAT64);
    EQ(Float64(f64s3), NIL_FLOAT64);

    Sample<Int8> i8Max(MAX_INT8);
    Sample<Int8> i8Min(MIN_INT8);
    Sample<Int8> i8Nil(NIL_INT8);
    Sample<Int8> i8s1(i8Max);
    Sample<Int8> i8s2(i8Min);
    Sample<Int8> i8s3(i8Nil);
    EQ(Int8(i8s1), MAX_INT8);
    EQ(Int8(i8s2), MIN_INT8);
    EQ(Int8(i8s3), NIL_INT8);

    Sample<Int16> i16Max(MAX_INT16);
    Sample<Int16> i16Min(MIN_INT16);
    Sample<Int16> i16Nil(NIL_INT16);
    Sample<Int16> i16s1(i16Max);
    Sample<Int16> i16s2(i16Min);
    Sample<Int16> i16s3(i16Nil);
    EQ(Int16(i16s1), MAX_INT16);
    EQ(Int16(i16s2), MIN_INT16);
    EQ(Int16(i16s3), NIL_INT16);

    Sample<Int32> i32Max(MAX_INT32);
    Sample<Int32> i32Min(MIN_INT32);
    Sample<Int32> i32Nil(NIL_INT32);
    Sample<Int32> i32s1(i32Max);
    Sample<Int32> i32s2(i32Min);
    Sample<Int32> i32s3(i32Nil);
    EQ(Int32(i32s1), MAX_INT32);
    EQ(Int32(i32s2), MIN_INT32);
    EQ(Int32(i32s3), NIL_INT32);
}

TEST(SoundSampleTest, ctorConvert)
{
    Sample<Float32> f32Max(MAX_FLOAT32);
    Sample<Float32> f32Min(MIN_FLOAT32);
    Sample<Float32> f32Nil(NIL_FLOAT32);

    Sample<Float64> f64Max(MAX_FLOAT64);
    Sample<Float64> f64Min(MIN_FLOAT64);
    Sample<Float64> f64Nil(NIL_FLOAT64);

    Sample<Int8> i8Max(MAX_INT8);
    Sample<Int8> i8Min(MIN_INT8);
    Sample<Int8> i8Nil(NIL_INT8);

    Sample<Int16> i16Max(MAX_INT16);
    Sample<Int16> i16Min(MIN_INT16);
    Sample<Int16> i16Nil(NIL_INT16);

    Sample<Int32> i32Max(MAX_INT32);
    Sample<Int32> i32Min(MIN_INT32);
    Sample<Int32> i32Nil(NIL_INT32);

    // Float32
    Sample<Float32> f32_from_f32Max(f32Max);
    Sample<Float32> f32_from_f64Max(f64Max);
    Sample<Float32> f32_from_i8Max(i8Max);
    Sample<Float32> f32_from_i16Max(i16Max);
    Sample<Float32> f32_from_i32Max(i32Max);
    EQ(f32_from_f32Max.value(), MAX_FLOAT32);
    EQ(f32_from_f64Max.value(), MAX_FLOAT32);
    EQ(f32_from_i8Max.value(), MAX_FLOAT32);
    EQ(f32_from_i16Max.value(), MAX_FLOAT32);
    EQ(f32_from_i32Max.value(), MAX_FLOAT32);

    Sample<Float32> f32_from_f32Min(f32Min);
    Sample<Float32> f32_from_f64Min(f64Min);
    Sample<Float32> f32_from_i8Min(i8Min);
    Sample<Float32> f32_from_i16Min(i16Min);
    Sample<Float32> f32_from_i32Min(i32Min);
    EQ(f32_from_f32Min.value(), MIN_FLOAT32);
    EQ(f32_from_f64Min.value(), MIN_FLOAT32);
    EQ(f32_from_i8Min.value(), MIN_FLOAT32);
    EQ(f32_from_i16Min.value(), MIN_FLOAT32);
    EQ(f32_from_i32Min.value(), MIN_FLOAT32);

    Sample<Float32> f32_from_f32Nil(f32Nil);
    Sample<Float32> f32_from_f64Nil(f64Nil);
    Sample<Float32> f32_from_i8Nil(i8Nil);
    Sample<Float32> f32_from_i16Nil(i16Nil);
    Sample<Float32> f32_from_i32Nil(i32Nil);
    EQ(f32_from_f32Nil.value(), NIL_FLOAT32);
    EQ(f32_from_f64Nil.value(), NIL_FLOAT32);
    EQ(f32_from_i8Nil.value(), NIL_FLOAT32);
    EQ(f32_from_i16Nil.value(), NIL_FLOAT32);
    EQ(f32_from_i32Nil.value(), NIL_FLOAT32);

    // Float64
    Sample<Float64> f64_from_f32Max(f32Max);
    Sample<Float64> f64_from_f64Max(f64Max);
    Sample<Float64> f64_from_i8Max(i8Max);
    Sample<Float64> f64_from_i16Max(i16Max);
    Sample<Float64> f64_from_i32Max(i32Max);
    EQ(f64_from_f64Max.value(), MAX_FLOAT64);
    EQ(f64_from_f64Max.value(), MAX_FLOAT64);
    EQ(f64_from_i8Max.value(), MAX_FLOAT64);
    EQ(f64_from_i16Max.value(), MAX_FLOAT64);
    EQ(f64_from_i32Max.value(), MAX_FLOAT64);

    Sample<Float64> f64_from_f32Min(f32Min);
    Sample<Float64> f64_from_f64Min(f64Min);
    Sample<Float64> f64_from_i8Min(i8Min);
    Sample<Float64> f64_from_i16Min(i16Min);
    Sample<Float64> f64_from_i32Min(i32Min);
    EQ(f64_from_f64Min.value(), MIN_FLOAT64);
    EQ(f64_from_f64Min.value(), MIN_FLOAT64);
    EQ(f64_from_i8Min.value(), MIN_FLOAT64);
    EQ(f64_from_i16Min.value(), MIN_FLOAT64);
    EQ(f64_from_i32Min.value(), MIN_FLOAT64);

    Sample<Float64> f64_from_f32Nil(f32Nil);
    Sample<Float64> f64_from_f64Nil(f64Nil);
    Sample<Float64> f64_from_i8Nil(i8Nil);
    Sample<Float64> f64_from_i16Nil(i16Nil);
    Sample<Float64> f64_from_i32Nil(i32Nil);
    EQ(f64_from_f64Nil.value(), NIL_FLOAT64);
    EQ(f64_from_f64Nil.value(), NIL_FLOAT64);
    EQ(f64_from_i8Nil.value(), NIL_FLOAT64);
    EQ(f64_from_i16Nil.value(), NIL_FLOAT64);
    EQ(f64_from_i32Nil.value(), NIL_FLOAT64);

    for (int i = 0; i < CYCLES; i++) {
        // Int8 (always dithering)
        Sample<Int8> i8_from_f32Max(f32Max);
        Sample<Int8> i8_from_f64Max(f64Max);
        Sample<Int8> i8_from_i8Max(i8Max);
        Sample<Int8> i8_from_i16Max(i16Max);
        Sample<Int8> i8_from_i32Max(i32Max);
        NEAR(i8_from_f32Max.value(), MAX_INT8, LSB_ERROR);
        NEAR(i8_from_f64Max.value(), MAX_INT8, LSB_ERROR);
        EQ(i8_from_i8Max.value(), MAX_INT8);
        NEAR(i8_from_i16Max.value(), MAX_INT8, LSB_ERROR);
        NEAR(i8_from_i32Max.value(), MAX_INT8, LSB_ERROR);

        Sample<Int8> i8_from_f32Min(f32Min);
        Sample<Int8> i8_from_f64Min(f64Min);
        Sample<Int8> i8_from_i8Min(i8Min);
        Sample<Int8> i8_from_i16Min(i16Min);
        Sample<Int8> i8_from_i32Min(i32Min);
        NEAR(i8_from_f32Min.value(), MIN_INT8, LSB_ERROR);
        NEAR(i8_from_f64Min.value(), MIN_INT8, LSB_ERROR);
        EQ(i8_from_i8Min.value(), MIN_INT8);
        NEAR(i8_from_i16Min.value(), MIN_INT8, LSB_ERROR);
        NEAR(i8_from_i32Min.value(), MIN_INT8, LSB_ERROR);

        Sample<Int8> i8_from_f32Nil(f32Nil);
        Sample<Int8> i8_from_f64Nil(f64Nil);
        Sample<Int8> i8_from_i8Nil(i8Nil);
        Sample<Int8> i8_from_i16Nil(i16Nil);
        Sample<Int8> i8_from_i32Nil(i32Nil);
        NEAR(i8_from_f32Nil.value(), NIL_INT8, LSB_ERROR);
        NEAR(i8_from_f64Nil.value(), NIL_INT8, LSB_ERROR);
        EQ(i8_from_i8Nil.value(), NIL_INT8);
        NEAR(i8_from_i16Nil.value(), NIL_INT8, LSB_ERROR);
        NEAR(i8_from_i32Nil.value(), NIL_INT8, LSB_ERROR);

        // Int16 (some dithering)
        Sample<Int16> i16_from_f32Max(f32Max);
        Sample<Int16> i16_from_f64Max(f64Max);
        Sample<Int16> i16_from_i8Max(i8Max);
        Sample<Int16> i16_from_i16Max(i16Max);
        Sample<Int16> i16_from_i32Max(i32Max);
        NEAR(i16_from_f32Max.value(), MAX_INT16, LSB_ERROR);
        NEAR(i16_from_f64Max.value(), MAX_INT16, LSB_ERROR);
        EQ(i16_from_i8Max.value(), MAX_INT16);
        EQ(i16_from_i16Max.value(), MAX_INT16);
        NEAR(i16_from_i32Max.value(), MAX_INT16, LSB_ERROR);

        Sample<Int16> i16_from_f32Min(f32Min);
        Sample<Int16> i16_from_f64Min(f64Min);
        Sample<Int16> i16_from_i8Min(i8Min);
        Sample<Int16> i16_from_i16Min(i16Min);
        Sample<Int16> i16_from_i32Min(i32Min);
        NEAR(i16_from_f32Min.value(), MIN_INT16, LSB_ERROR);
        NEAR(i16_from_f64Min.value(), MIN_INT16, LSB_ERROR);
        EQ(i16_from_i8Min.value(), MIN_INT16);
        EQ(i16_from_i16Min.value(), MIN_INT16);
        NEAR(i16_from_i32Min.value(), MIN_INT16, LSB_ERROR);

        Sample<Int16> i16_from_f32Nil(f32Nil);
        Sample<Int16> i16_from_f64Nil(f64Nil);
        Sample<Int16> i16_from_i8Nil(i8Nil);
        Sample<Int16> i16_from_i16Nil(i16Nil);
        Sample<Int16> i16_from_i32Nil(i32Nil);
        NEAR(i16_from_f32Nil.value(), NIL_INT16, LSB_ERROR);
        NEAR(i16_from_f64Nil.value(), NIL_INT16, LSB_ERROR);
        EQ(i16_from_i8Nil.value(), NIL_INT16);
        EQ(i16_from_i16Nil.value(), NIL_INT16);
        NEAR(i16_from_i32Nil.value(), NIL_INT16, LSB_ERROR);

        // Int32 (a few dithering)
        Sample<Int32> i32_from_f32Max(f32Max);
        Sample<Int32> i32_from_f64Max(f64Max);
        Sample<Int32> i32_from_i8Max(i8Max);
        Sample<Int32> i32_from_i16Max(i16Max);
        Sample<Int32> i32_from_i32Max(i32Max);
        EQ(i32_from_f32Max.value(), MAX_INT32);
        NEAR(i32_from_f64Max.value(), MAX_INT32, LSB_ERROR);
        EQ(i32_from_i8Max.value(), MAX_INT32);
        EQ(i32_from_i16Max.value(), MAX_INT32);
        EQ(i32_from_i32Max.value(), MAX_INT32);

        Sample<Int32> i32_from_f32Min(f32Min);
        Sample<Int32> i32_from_f64Min(f64Min);
        Sample<Int32> i32_from_i8Min(i8Min);
        Sample<Int32> i32_from_i16Min(i16Min);
        Sample<Int32> i32_from_i32Min(i32Min);
        EQ(i32_from_f32Min.value(), MIN_INT32);
        NEAR(i32_from_f64Min.value(), MIN_INT32, LSB_ERROR);
        EQ(i32_from_i8Min.value(), MIN_INT32);
        EQ(i32_from_i16Min.value(), MIN_INT32);
        EQ(i32_from_i32Min.value(), MIN_INT32);

        Sample<Int32> i32_from_f32Nil(f32Nil);
        Sample<Int32> i32_from_f64Nil(f64Nil);
        Sample<Int32> i32_from_i8Nil(i8Nil);
        Sample<Int32> i32_from_i16Nil(i16Nil);
        Sample<Int32> i32_from_i32Nil(i32Nil);
        EQ(i32_from_f32Nil.value(), NIL_INT32);
        NEAR(i32_from_f64Nil.value(), NIL_INT32, LSB_ERROR);
        EQ(i32_from_i8Nil.value(), NIL_INT32);
        EQ(i32_from_i16Nil.value(), NIL_INT32);
        EQ(i32_from_i32Nil.value(), NIL_INT32);
    }
}

TEST(SoundSampleTest, max)
{
    NO_THROW(Sample<Float32>::max());
    NO_THROW(Sample<Float64>::max());
    NO_THROW(Sample<Int8>::max());
    NO_THROW(Sample<Int16>::max());
    NO_THROW(Sample<Int32>::max());

    EQ(Sample<Float32>::max(), MAX_FLOAT32);
    EQ(Sample<Float64>::max(), MAX_FLOAT64);
    EQ(Sample<Int8>::max(), MAX_INT8);
    EQ(Sample<Int16>::max(), MAX_INT16);
    EQ(Sample<Int32>::max(), MAX_INT32);

    Sample<Float32> f32;
    Sample<Float64> f64;
    Sample<Int8> i8;
    Sample<Int16> i16;
    Sample<Int32> i32;

    NO_THROW(f32.max());
    NO_THROW(f64.max());
    NO_THROW(i8.max());
    NO_THROW(i16.max());
    NO_THROW(i32.max());

    EQ(f32.max(), MAX_FLOAT32);
    EQ(f64.max(), MAX_FLOAT64);
    EQ(i8.max(), MAX_INT8);
    EQ(i16.max(), MAX_INT16);
    EQ(i32.max(), MAX_INT32);
}

TEST(SoundSampleTest, min)
{
    NO_THROW(Sample<Float32>::min());
    NO_THROW(Sample<Float64>::min());
    NO_THROW(Sample<Int8>::min());
    NO_THROW(Sample<Int16>::min());
    NO_THROW(Sample<Int32>::min());

    EQ(Sample<Float32>::min(), MIN_FLOAT32);
    EQ(Sample<Float64>::min(), MIN_FLOAT64);
    EQ(Sample<Int8>::min(), MIN_INT8);
    EQ(Sample<Int16>::min(), MIN_INT16);
    EQ(Sample<Int32>::min(), MIN_INT32);

    Sample<Float32> f32;
    Sample<Float64> f64;
    Sample<Int8> i8;
    Sample<Int16> i16;
    Sample<Int32> i32;

    NO_THROW(f32.min());
    NO_THROW(f64.min());
    NO_THROW(i8.min());
    NO_THROW(i16.min());
    NO_THROW(i32.min());

    EQ(f32.min(), MIN_FLOAT32);
    EQ(f64.min(), MIN_FLOAT64);
    EQ(i8.min(), MIN_INT8);
    EQ(i16.min(), MIN_INT16);
    EQ(i32.min(), MIN_INT32);
}

TEST(SoundSampleTest, nil)
{
    NO_THROW(Sample<Float32>::nil());
    NO_THROW(Sample<Float64>::nil());
    NO_THROW(Sample<Int8>::nil());
    NO_THROW(Sample<Int16>::nil());
    NO_THROW(Sample<Int32>::nil());

    EQ(Sample<Float32>::nil(), NIL_FLOAT32);
    EQ(Sample<Float64>::nil(), NIL_FLOAT64);
    EQ(Sample<Int8>::nil(), NIL_INT8);
    EQ(Sample<Int16>::nil(), NIL_INT16);
    EQ(Sample<Int32>::nil(), NIL_INT32);

    Sample<Float32> f32;
    Sample<Float64> f64;
    Sample<Int8> i8;
    Sample<Int16> i16;
    Sample<Int32> i32;

    NO_THROW(f32.nil());
    NO_THROW(f64.nil());
    NO_THROW(i8.nil());
    NO_THROW(i16.nil());
    NO_THROW(i32.nil());

    EQ(f32.nil(), NIL_FLOAT32);
    EQ(f64.nil(), NIL_FLOAT64);
    EQ(i8.nil(), NIL_INT8);
    EQ(i16.nil(), NIL_INT16);
    EQ(i32.nil(), NIL_INT32);
}

TEST(SoundSampleTest, isFloat)
{
    NO_THROW(Sample<Float32>::isFloat());
    NO_THROW(Sample<Float64>::isFloat());
    NO_THROW(Sample<Int8>::isFloat());
    NO_THROW(Sample<Int16>::isFloat());
    NO_THROW(Sample<Int32>::isFloat());

    EQ(Sample<Float32>::isFloat(), true);
    EQ(Sample<Float64>::isFloat(), true);
    EQ(Sample<Int8>::isFloat(), false);
    EQ(Sample<Int16>::isFloat(), false);
    EQ(Sample<Int32>::isFloat(), false);

    Sample<Float32> f32;
    Sample<Float64> f64;
    Sample<Int8> i8;
    Sample<Int16> i16;
    Sample<Int32> i32;

    NO_THROW(f32.isFloat());
    NO_THROW(f64.isFloat());
    NO_THROW(i8.isFloat());
    NO_THROW(i16.isFloat());
    NO_THROW(i32.isFloat());

    EQ(f32.isFloat(), true);
    EQ(f64.isFloat(), true);
    EQ(i8.isFloat(), false);
    EQ(i16.isFloat(), false);
    EQ(i32.isFloat(), false);
}

TEST(SoundSampleTest, isInt)
{
    NO_THROW(Sample<Float32>::isInt());
    NO_THROW(Sample<Float64>::isInt());
    NO_THROW(Sample<Int8>::isInt());
    NO_THROW(Sample<Int16>::isInt());
    NO_THROW(Sample<Int32>::isInt());

    EQ(Sample<Float32>::isInt(), false);
    EQ(Sample<Float64>::isInt(), false);
    EQ(Sample<Int8>::isInt(), true);
    EQ(Sample<Int16>::isInt(), true);
    EQ(Sample<Int32>::isInt(), true);

    Sample<Float32> f32;
    Sample<Float64> f64;
    Sample<Int8> i8;
    Sample<Int16> i16;
    Sample<Int32> i32;

    NO_THROW(f32.isInt());
    NO_THROW(f64.isInt());
    NO_THROW(i8.isInt());
    NO_THROW(i16.isInt());
    NO_THROW(i32.isInt());

    EQ(f32.isInt(), false);
    EQ(f64.isInt(), false);
    EQ(i8.isInt(), true);
    EQ(i16.isInt(), true);
    EQ(i32.isInt(), true);
}

TEST(SoundSampleTest, value)
{
    Sample<Float32> f32Max(MAX_FLOAT32);
    Sample<Float32> f32Min(MIN_FLOAT32);
    Sample<Float32> f32Nil(NIL_FLOAT32);
    EQ(Float32(f32Max), MAX_FLOAT32);
    EQ(Float32(f32Min), MIN_FLOAT32);
    EQ(Float32(f32Nil), NIL_FLOAT32);

    Sample<Float64> f64Max(MAX_FLOAT64);
    Sample<Float64> f64Min(MIN_FLOAT64);
    Sample<Float64> f64Nil(NIL_FLOAT64);
    EQ(Float64(f64Max), MAX_FLOAT64);
    EQ(Float64(f64Min), MIN_FLOAT64);
    EQ(Float64(f64Nil), NIL_FLOAT64);

    Sample<Int8> i8Max(MAX_INT8);
    Sample<Int8> i8Min(MIN_INT8);
    Sample<Int8> i8Nil(NIL_INT8);
    EQ(Int8(i8Max), MAX_INT8);
    EQ(Int8(i8Min), MIN_INT8);
    EQ(Int8(i8Nil), NIL_INT8);

    Sample<Int16> i16Max(MAX_INT16);
    Sample<Int16> i16Min(MIN_INT16);
    Sample<Int16> i16Nil(NIL_INT16);
    EQ(Int16(i16Max), MAX_INT16);
    EQ(Int16(i16Min), MIN_INT16);
    EQ(Int16(i16Nil), NIL_INT16);

    Sample<Int32> i32Max(MAX_INT32);
    Sample<Int32> i32Min(MIN_INT32);
    Sample<Int32> i32Nil(NIL_INT32);
    EQ(Int32(i32Max), MAX_INT32);
    EQ(Int32(i32Min), MIN_INT32);
    EQ(Int32(i32Nil), NIL_INT32);
}
