#ifndef GENERATOR_TEST_H
#define GENERATOR_TEST_H

#include "common_test.h"
#include "object.h"
#include <random>

using namespace Sound;

class GeneratorTest : public ::testing::Test {
public:
    constexpr static unsigned LSB_ERROR = 2;
    constexpr static Precise SAMPLE_RATE = 44100;
    constexpr static unsigned TEST_LENGTH = int(SAMPLE_RATE) * 2;
    constexpr static Precise TEST_FREQUENCY = 440;

    GeneratorTest()
        : _dev()
        , _gen(_dev())
        , _dis(-1, 1)
    {
    }

    ~GeneratorTest() {}

    Precise random()
    {
        return _dis(_gen);
    }

    Precise sine(unsigned sample, Precise frequency = TEST_FREQUENCY)
    {
        return std::sin(2.L * M_PI * frequency * sample / SAMPLE_RATE) / 2;
    }

private:
    std::random_device _dev;
    std::mt19937_64 _gen;
    std::uniform_real_distribution<Precise> _dis;
};

#endif // GENERATOR_TEST_H
