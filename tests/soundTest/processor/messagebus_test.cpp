#include <gtest/gtest.h>

#include "sound/config.h"
#include "sound/processor/messagebus.h"
#include "sound/processor/processor.h"

#define PROCESSOR TestProcessor
#include "sound/processor/shortcuts.h"

namespace Sound {
SOUND_PROCESSOR_COMMANDS(TestProcessor, Processor,
    TestCommand1,
    TestCommand2,
    TestCommand3);
SOUND_PROCESSOR_PARAMETERS(TestProcessor, Processor,
    Param1Bool,
    Param2Float,
    Param3Int,
    Param4Bool,
    Param5Str,
    Param6Ptr,
    Param7UInt);
SOUND_PROCESSOR_PROPERTIES(TestProcessor, Processor, Void);
SOUND_PROCESSOR_SIGNALS(TestProcessor, Processor, Void);
}

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
class TestProcessor : public Processor<T> {
public:
    TestProcessor()
        : _command1Passed(0)
        , _command2Passed(0)
        , _initsPassed(0)
    {
        INITIALIZE;

        COMMAND(TestCommand1);
        COMMAND(TestCommand2);
        COMMAND(TestCommand3);

        PARAMETER(bool, Param1Bool, false);
        PARAMETER(float, Param2Float, 0.4f);
        PARAMETER(int, Param3Int, -135);
        PARAMETER(bool, Param4Bool, true);
        PARAMETER(const char *, Param5Str, "test string");
        PARAMETER(void *, Param6Ptr, NULL);
        PARAMETER(unsigned, Param7UInt, 0);
    }

    void process() override {}

    unsigned command1Passed() const { return _command1Passed; }
    unsigned command2Passed() const { return _command2Passed; }
    unsigned initsPassed() const { return _initsPassed; }

protected:
    virtual void commandTestCommand1() { ++_command1Passed; }
    virtual void commandTestCommand2() { ++_command2Passed; }
    virtual void commandTestCommand3() { throw std::out_of_range("Test exception"); }
    void commandInit() override { ++_initsPassed; }

private:
    unsigned _command1Passed;
    unsigned _command2Passed;
    unsigned _initsPassed;
};

typedef TestProcessor<Float32> TProc;

void testWatcher1(unsigned, Signal::ID, Value)
{
}

void testWatcher2(unsigned, Signal::ID, Value)
{
}

void testWatcherThrow(unsigned, Signal::ID, Value)
{
    throw std::out_of_range("test");
}

TEST(SoundProcessorMessageBusTest, ctor)
{
    EXPECT_NO_THROW(MessageBus bus);

    MessageBus bus;
    EXPECT_NO_THROW(bus.clear());
    EXPECT_NO_THROW(bus.dispatch());
    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_NO_THROW(bus.passParameter(0, Parameter::ID(), 0));
    EXPECT_NO_THROW(bus.requireCommand(0, Command::ID()));
    EXPECT_NO_THROW(bus.realtimeEmitSignal(0, Signal::ID(), 0));
}

TEST(SoundProcessorMessageBusTest, addRemoveProcessor)
{
    MessageBus bus;
    EXPECT_ANY_THROW(bus.removeProcessor(0));
    EXPECT_ANY_THROW(bus.removeProcessor(100));
    EXPECT_ANY_THROW(bus.removeProcessor(unsigned(-1)));
    EXPECT_ANY_THROW(bus.addProcessor(NULL));

    auto processor1 = std::make_shared<TProc>();
    auto processor2 = std::make_shared<TProc>();
    auto processor3 = std::make_shared<TProc>();

    EXPECT_NO_THROW(bus.addProcessor(processor1.get()));
    EXPECT_NO_THROW(bus.addProcessor(processor2.get()));
    EXPECT_NO_THROW(bus.addProcessor(processor3.get()));

    EXPECT_ANY_THROW(bus.removeProcessor(20));
    EXPECT_NO_THROW(bus.removeProcessor(processor2->id()));
    EXPECT_ANY_THROW(bus.removeProcessor(processor2->id()));

    EXPECT_NO_THROW(bus.removeProcessor(processor1->id()));
    EXPECT_NO_THROW(bus.removeProcessor(processor3->id()));
    EXPECT_ANY_THROW(bus.removeProcessor(processor1->id()));
}

TEST(SoundProcessorMessageBusTest, addRemoveWatcher)
{
    MessageBus bus;
    EXPECT_ANY_THROW(bus.removeWatcher(testWatcher1));
    EXPECT_ANY_THROW(bus.removeWatcher(nullptr));
    EXPECT_ANY_THROW(bus.removeWatcher(NULL));
    EXPECT_ANY_THROW(bus.addWatcher(NULL));

    auto processor = std::make_shared<TProc>();

    EXPECT_NO_THROW(bus.addWatcher(testWatcher1));
    EXPECT_NO_THROW(bus.addWatcher(testWatcher2, processor->id()));
    EXPECT_NO_THROW(bus.addWatcher(testWatcherThrow, Signal::Processor::Error));

    EXPECT_NO_THROW(bus.removeWatcher(testWatcher1));
    EXPECT_ANY_THROW(bus.removeWatcher(testWatcher1));
    EXPECT_NO_THROW(bus.removeWatcher(testWatcher2));
    EXPECT_NO_THROW(bus.removeWatcher(testWatcherThrow));
}

TEST(SoundProcessorMessageBusTest, commands)
{
    MessageBus bus;

    auto processor = std::make_shared<TProc>();
    bus.addProcessor(processor.get());

    // a few commands
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::Processor::Init));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::Processor::Init));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::Processor::Init));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::Processor::Init));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::TestProcessor::TestCommand1));

    EXPECT_NO_THROW(bus.dispatch());
    EXPECT_EQ(processor->initsPassed(), 0);
    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 4);

    EXPECT_NO_THROW(processor->perform(Command::Processor::Init)); // direct
    EXPECT_EQ(processor->initsPassed(), 5);

    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::TestProcessor::TestCommand1));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::TestProcessor::TestCommand2));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::TestProcessor::TestCommand3));
    EXPECT_NO_THROW(bus.requireCommand(processor->id(), Command::TestProcessor::TestCommand2));

    EXPECT_ANY_THROW(bus.realtimeDispatch()); // from commandTestCommand3()
    EXPECT_EQ(processor->initsPassed(), 5);
    EXPECT_EQ(processor->command1Passed(), 2);
    EXPECT_EQ(processor->command2Passed(), 1);
    EXPECT_NO_THROW(bus.realtimeDispatch()); // recover
    EXPECT_EQ(processor->command2Passed(), 2);

    // many commands
    int count = SOUND_PROCESSOR_COMMAND_BUS_SIZE * 8 + 26;
    int capacity = SOUND_PROCESSOR_COMMAND_BUS_SIZE - 1;

    for (int i = 1; i <= count; i++) {
        EXPECT_NO_THROW(
            bus.requireCommand(processor->id(), Command::Processor::Init));
    }

    EXPECT_EQ(processor->initsPassed(), 5);

    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + capacity);

    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + capacity); // not flushed
    EXPECT_NO_THROW(bus.dispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + capacity); // flushed, not dispatched
    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + 2 * capacity); // dispached

    EXPECT_NO_THROW(bus.dispatch());
    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + 3 * capacity);

    EXPECT_NO_THROW(bus.dispatch());
    EXPECT_NO_THROW(bus.realtimeDispatch());
    EXPECT_EQ(processor->initsPassed(), 5 + 4 * capacity);

    unsigned prevPassed;
    unsigned nowPassed = processor->initsPassed();

    do {
        EXPECT_NO_THROW(bus.dispatch());
        EXPECT_NO_THROW(bus.realtimeDispatch());
        prevPassed = nowPassed;
        nowPassed = processor->initsPassed();
    } while (prevPassed < nowPassed);

    EXPECT_EQ(processor->initsPassed(), 5 + count);
}

TEST(SoundProcessorMessageBusTest, parameters)
{
    MessageBus bus;

    auto processor = std::make_shared<TProc>();
    bus.addProcessor(processor.get());
}
