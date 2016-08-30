#include "common_test.h"

#include "sample.h"

#include "log4cplus/consoleappender.h"
#include "log4cplus/layout.h"
#include "log4cplus/logger.h"
#include "ltdl.h"
#include "main/config.h"
#include "shared/server.h"
#include "stream/outputstream.h"
#include <vector>

using namespace log4cplus;
using namespace Sound;

const Precise SAMPLE_RATE = 44100;
const Precise TEST_FREQUENCY = 440 * 8;
const int TEST_LENGTH = int(SAMPLE_RATE) * 5;

Precise sine(unsigned sample, Precise frequency = TEST_FREQUENCY)
{
    return std::sin(2.L * M_PI * frequency * sample / SAMPLE_RATE) / 2.0L;
}

//TEST(SoundDitherTest, tone)
//{
//    Buffer<Int16> buffer(1, TEST_LENGTH);
//    auto frame = buffer.begin();

//    for (unsigned i = 0; i < TEST_LENGTH; i++) {
//        Precise factor = Precise(i + 1) / Precise(TEST_LENGTH - 1);
//        Precise freq = factor * factor * factor * factor     * TEST_FREQUENCY;
//        Sample<Precise> precise = sine(i, freq);
//        Sample<Int8> sample(precise);
//        frame[0] = sample;
//        ++frame;
//    }

//#define LOG_PATTERN "%-5p *** %m%n[ %b:%L ] %M%n"

//    auto console = SharedAppenderPtr(new ConsoleAppender(true, true));
//    auto layout = std::auto_ptr<Layout>(new PatternLayout(LOG_PATTERN));
//    auto logger = Logger::getInstance(APPLICATION_NAME);
//    console->setLayout(layout);
//    logger.addAppender(console);
//    logger.setLogLevel(TRACE_LOG_LEVEL);

//    lt_dlinit();
//    Server::instance()->startup(APPLICATION_PLUGIN_PATH);

//    IAudioFile::FileInfo fi;
//    memset(&fi, 0, sizeof fi);
//    fi.channels = 1;
//    fi.sampleRate = 44100;
//    fi.sampleType = Type::Int16;
//    fi.format.major = IAudioFile::MajorWAV;
//    fi.format.minor = IAudioFile::MinorPCM_16;

//    OutputStream stream("/tmp/test_int16.wav", fi);
//    stream << buffer;
//}
