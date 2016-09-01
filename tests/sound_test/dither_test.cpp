#include "generator_test.h"

#include "buffer.h"
#include "sample.h"

class SoundDitherTest : public GeneratorTest {
public:
    static constexpr int MAX_REPEATS = 2;
};

using namespace Sound;

TEST_F(SoundDitherTest, tone)
{
    Buffer<Precise> reference(1, TEST_LENGTH);
    Buffer<Int8> i8(1, TEST_LENGTH);
    Buffer<Int16> i16(1, TEST_LENGTH);
    Buffer<Int32> i32(1, TEST_LENGTH);
    Buffer<Int64> i64(1, TEST_LENGTH);
    auto reff = reference.begin();
    auto i8f = i8.begin();
    auto i16f = i16.begin();
    auto i32f = i32.begin();
    auto i64f = i64.begin();

    for (unsigned i = 0; i < TEST_LENGTH; i++,
         ++reff, ++i8f, ++i16f, ++i32f, ++i64f) {
        Precise factor = Precise(i + 1) / Precise(TEST_LENGTH - 1);
        Precise freq = std::pow(factor, 5) * TEST_FREQUENCY * 8;
        Sample<Precise> precise = sine(i, freq);
        reff[0] = precise;
        i8f[0] = precise;
        i16f[0] = precise;
        i64f[0] = precise;
        i32f[0] = precise;
    }

    int i8RoundStat = 0, i16RoundStat = 0, i64RoundStat = 0, i32RoundStat = 0;
    int i8TruncStat = 0, i16TruncStat = 0, i64TruncStat = 0, i32TruncStat = 0;
    bool isBigPrecise = sizeof(Precise) > sizeof(Int64);

    for (unsigned rep = 0; rep < MAX_REPEATS; rep++) {
        auto refcf = reference.cbegin();
        auto i8cf = i8.cbegin();
        auto i16cf = i16.cbegin();
        auto i64cf = i64.cbegin();
        auto i32cf = i32.cbegin();

        for (unsigned i = 0; i < TEST_LENGTH; i++,
                      ++refcf, ++i8cf, ++i16cf, ++i32cf, ++i64cf) {
            Precise refv = refcf[0].value();
            Int8 i8v = i8cf[0].value();
            Int16 i16v = i16cf[0].value();
            Int64 i64v = i64cf[0].value();
            Int32 i32v = i32cf[0].value();

            Precise i8r, i16r, i64r, i32r;

            if (refv < 0) {
                i8r = -refv * (MIN_INT8 + 1);
                i16r = -refv * (MIN_INT16 + 1);
                i64r = -refv * (MIN_INT64 + isBigPrecise);
                i32r = -refv * (MIN_INT32 + 1);
            } else {
                i8r = refv * (MAX_INT8 - 1);
                i16r = refv * (MAX_INT16 - 1);
                i64r = refv * (MAX_INT64 - isBigPrecise);
                i32r = refv * (MAX_INT32 - 1);
            }

            ASSERT_NEAR(i8v, std::lroundl(i8r), LSB_ERROR);
            ASSERT_NEAR(i16v, std::lroundl(i16r), LSB_ERROR);
            ASSERT_NEAR(i32v, std::lroundl(i32r), LSB_ERROR);

            if (isBigPrecise) {
                ASSERT_NEAR(i64v, std::llroundl(i64r), LSB_ERROR);
            } else {
                ASSERT_EQ(i64v, std::llroundl(i64r));
            }

            i8RoundStat += i8v != std::lroundl(i8r);
            i16RoundStat += i16v != std::lroundl(i16r);
            i64RoundStat += i64v != std::lroundl(i64r);
            i32RoundStat += i32v != std::lroundl(i32r);

            i8TruncStat += i8v != Int8(i8r);
            i16TruncStat += i16v != Int16(i16r);
            i64TruncStat += i64v != Int64(i64r);
            i32TruncStat += i32v != Int32(i32r);
        }

        if (i8RoundStat && i16RoundStat && i32RoundStat
            && i8TruncStat && i16TruncStat && i32TruncStat
            && (isBigPrecise && i64RoundStat)) {
            break;
        }
    }

    TRUE(i8RoundStat && i16RoundStat && i32RoundStat
        && (!isBigPrecise || i64RoundStat))
        << "Dither seems to be never applied, round used";

    TRUE(i8TruncStat && i16TruncStat && i32TruncStat
        && (!isBigPrecise || i64TruncStat))
        << "Dither seems to be never applied, truncate used";
}
