//#include <gtest/gtest.h>

//#include "sound/config.h"
//#include "sound/processor/messagebus.h"
//#include "sound/processor/processor.h"

//#define PROCESSOR TestProcessor
//#include "sound/processor/shortcuts.h"

//namespace Sound {
//SOUND_PROCESSOR_COMMANDS(TestProcessor, Processor,
//    TestCommand1,
//    TestCommand2,
//    EmitSignal,
//    TestCommand3);
//SOUND_PROCESSOR_PARAMETERS(TestProcessor, Processor,
//    Param1Bool,
//    Param2Float,
//    Param3Int,
//    Param4Bool,
//    Param5Str,
//    Param6Ptr,
//    Param7UInt);
//SOUND_PROCESSOR_PROPERTIES(TestProcessor, Processor,
//    Void);
//SOUND_PROCESSOR_SIGNALS(TestProcessor, Processor,
//    TestSignal);
//}

//using namespace Sound;
//using namespace Sound::Processor;

//template <typename T>
//class TestProcessor : public Processor<T> {
//public:
//    TestProcessor()
//        : _command1Passed(0)
//        , _command2Passed(0)
//        , _initsPassed(0)
//    {
//        INITIALIZE;

//        COMMAND(TestCommand1);
//        COMMAND(TestCommand2);
//        COMMAND(TestCommand3);
//        COMMAND(EmitSignal);

//        PARAMETER(bool, Param1Bool, false);
//        PARAMETER(float, Param2Float, 0.4f);
//        PARAMETER(int, Param3Int, -135);
//        PARAMETER(bool, Param4Bool, true);
//        PARAMETER(const char*, Param5Str, "test string");
//        PARAMETER(const void*, Param6Ptr, nullptr);
//        PARAMETER(unsigned, Param7UInt, -1);
//    }

//    void process() override {}

//    unsigned command1Passed() const { return _command1Passed; }
//    unsigned command2Passed() const { return _command2Passed; }
//    unsigned initsPassed() const { return _initsPassed; }

//protected:
//    virtual void commandTestCommand1() { ++_command1Passed; }
//    virtual void commandTestCommand2() { ++_command2Passed; }
//    virtual void commandTestCommand3() { throw std::out_of_range("Test exception"); }
//    virtual void commandEmitSignal() {
//        static int sent = 0;
//        ++sent;
//        EXPECT_NO_THROW(this->send(Signal::TestProcessor::TestSignal, sent));
//    }

//    void commandInit() override { ++_initsPassed; }

//private:
//    unsigned _command1Passed;
//    unsigned _command2Passed;
//    unsigned _initsPassed;
//};

//typedef TestProcessor<Float32> TProcessor;

//void testWatcher1(unsigned, Signal::ID, Value)
//{
//}

//void testWatcher2(unsigned, Signal::ID, Value)
//{
//}

//void testWatcherThrow(unsigned, Signal::ID, Value)
//{
//    throw std::out_of_range("test");
//}

//TEST(SoundProcessorMessageBusTest, ctor)
//{
//    EXPECT_NO_THROW(MessageBus bus);

//    MessageBus bus;
//    EXPECT_NO_THROW(bus.clearBus());
//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_NO_THROW(bus.sendParameter(0, Parameter::ID(), 0));
//    EXPECT_NO_THROW(bus.sendCommand(0, Command::ID()));
//    EXPECT_NO_THROW(bus.realtimeEmitSignal(0, Signal::ID(), 0));
//}

//TEST(SoundProcessorMessageBusTest, addRemoveProcessor)
//{
//    MessageBus bus;
//    EXPECT_ANY_THROW(bus.removeProcessor(0));
//    EXPECT_ANY_THROW(bus.removeProcessor(100));
//    EXPECT_ANY_THROW(bus.removeProcessor(unsigned(-1)));
//    EXPECT_ANY_THROW(bus.addProcessor(NULL));

//    auto processor1 = std::make_shared<TProcessor>();
//    auto processor2 = std::make_shared<TProcessor>();
//    auto processor3 = std::make_shared<TProcessor>();

//    EXPECT_NO_THROW(bus.addProcessor(processor1.get()));
//    EXPECT_NO_THROW(bus.addProcessor(processor2.get()));
//    EXPECT_NO_THROW(bus.addProcessor(processor3.get()));

//    EXPECT_ANY_THROW(bus.removeProcessor(20));
//    EXPECT_NO_THROW(bus.removeProcessor(processor2->id()));
//    EXPECT_ANY_THROW(bus.removeProcessor(processor2->id()));

//    EXPECT_NO_THROW(bus.removeProcessor(processor1->id()));
//    EXPECT_NO_THROW(bus.removeProcessor(processor3->id()));
//    EXPECT_ANY_THROW(bus.removeProcessor(processor1->id()));
//}

//TEST(SoundProcessorMessageBusTest, addRemoveWatcher)
//{
//    MessageBus bus;
//    EXPECT_ANY_THROW(bus.removeWatcher(testWatcher1));
//    EXPECT_ANY_THROW(bus.removeWatcher(nullptr));
//    EXPECT_ANY_THROW(bus.removeWatcher(NULL));
//    EXPECT_ANY_THROW(bus.addWatcher(NULL));

//    auto processor = std::make_shared<TProcessor>();

//    EXPECT_NO_THROW(bus.addWatcher(testWatcher1));
//    EXPECT_NO_THROW(bus.addWatcher(testWatcher1, processor->id()));
//    EXPECT_NO_THROW(bus.addWatcher(testWatcher2, processor->id()));
//    EXPECT_NO_THROW(bus.addWatcher(testWatcherThrow, Signal::Processor::Error));

//    EXPECT_ANY_THROW(bus.addWatcher(testWatcher1));
//    EXPECT_ANY_THROW(bus.addWatcher(testWatcher1));
//    EXPECT_ANY_THROW(bus.addWatcher(testWatcher1));
//    EXPECT_ANY_THROW(bus.addWatcher(testWatcher1, processor->id()));
//    EXPECT_ANY_THROW(bus.addWatcher(testWatcher2, processor->id()));

//    EXPECT_NO_THROW(bus.removeWatcher(testWatcher1));
//    EXPECT_ANY_THROW(bus.removeWatcher(testWatcher1));
//    EXPECT_NO_THROW(bus.addWatcher(testWatcher1));

//    EXPECT_NO_THROW(bus.removeWatcher(testWatcher2));
//    EXPECT_NO_THROW(bus.removeWatcher(testWatcherThrow));

//    EXPECT_ANY_THROW(bus.removeWatcher(testWatcher2));
//    EXPECT_ANY_THROW(bus.removeWatcher(testWatcherThrow));
//}

//TEST(SoundProcessorMessageBusTest, commands)
//{
//    MessageBus bus;

//    auto processor = std::make_shared<TProcessor>();
//    bus.addProcessor(processor.get());

//    // a few commands
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::Processor::Init));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::Processor::Init));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::Processor::Init));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::Processor::Init));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::TestProcessor::TestCommand1));

//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_EQ(processor->initsPassed(), 0);
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 4);

//    EXPECT_NO_THROW(processor->perform(Command::Processor::Init)); // direct
//    EXPECT_EQ(processor->initsPassed(), 5);

//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::TestProcessor::TestCommand1));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::TestProcessor::TestCommand2));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::TestProcessor::TestCommand3));
//    EXPECT_NO_THROW(bus.sendCommand(processor->id(), Command::TestProcessor::TestCommand2));

//    EXPECT_ANY_THROW(bus.realtimeDispatch()); // from commandTestCommand3()
//    EXPECT_EQ(processor->initsPassed(), 5);
//    EXPECT_EQ(processor->command1Passed(), 2);
//    EXPECT_EQ(processor->command2Passed(), 1);
//    EXPECT_NO_THROW(bus.realtimeDispatch()); // recover
//    EXPECT_EQ(processor->command2Passed(), 2);

//    // many commands (no stress)
//    int count = SOUND_PROCESSOR_BUS_SIZE * (SOUND_PROCESSOR_BUS_HOLD_FACTOR - 2);
//    int capacity = SOUND_PROCESSOR_BUS_SIZE - 1;

//    for (int i = 1; i <= count; i++) {
//        EXPECT_NO_THROW(
//            bus.sendCommand(processor->id(), Command::Processor::Init));
//    }

//    EXPECT_EQ(processor->initsPassed(), 5);

//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + capacity);

//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + capacity); // not flushed
//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + capacity); // flushed, not dispatched
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + 2 * capacity); // dispached

//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + 3 * capacity);

//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_EQ(processor->initsPassed(), 5 + 4 * capacity);

//    unsigned prevPassed;
//    unsigned nowPassed = processor->initsPassed();

//    do {
//        EXPECT_NO_THROW(bus.dispatch());
//        EXPECT_NO_THROW(bus.realtimeDispatch());
//        prevPassed = nowPassed;
//        nowPassed = processor->initsPassed();
//    } while (prevPassed < nowPassed);

//    EXPECT_EQ(processor->initsPassed(), 5 + count);
//}

//TEST(SoundProcessorMessageBusTest, commandsStress)
//{
//    MessageBus bus;
//    auto processor = std::make_shared<TestProcessor<float> >();
//    bus.addProcessor(processor.get());

//    int count = SOUND_PROCESSOR_BUS_SIZE * SOUND_PROCESSOR_BUS_HOLD_FACTOR * 3;
//    EXPECT_EQ(processor->initsPassed(), 0);

//    EXPECT_NO_THROW(
//        for (int i = 1; i <= count; i++) {
//            bus.sendCommand(processor->id(), Command::Processor::Init);
//        });

//    EXPECT_NE(processor->initsPassed(), 0); // offloaded

//    unsigned prevPassed;
//    unsigned nowPassed = processor->initsPassed();

//    do {
//        EXPECT_NO_THROW(bus.dispatch());
//        EXPECT_NO_THROW(bus.realtimeDispatch());
//        prevPassed = nowPassed;
//        nowPassed = processor->initsPassed();
//    } while (prevPassed < nowPassed);

//    EXPECT_EQ(processor->initsPassed(), count);
//}

//TEST(SoundProcessorMessageBusTest, parameters)
//{
//    typedef Parameter::TestProcessor P;

//    MessageBus bus;

//    auto pr = std::make_shared<TProcessor>();
//    bus.addProcessor(pr.get());

//    EXPECT_EQ(pr->get(P::Param2Float).as<float>(), 0.4f);
//    EXPECT_EQ(pr->get(P::Param3Int).as<int>(), -135);
//    EXPECT_EQ(pr->get(P::Param4Bool).as<bool>(), true);
//    EXPECT_STREQ(pr->get(P::Param5Str).as<const char*>(), "test string");
//    EXPECT_EQ(pr->get(P::Param6Ptr).as<const void*>(), nullptr);
//    EXPECT_EQ(pr->get(P::Param7UInt).as<unsigned>(), -1);

//    EXPECT_EQ(pr->get(P::Param6Ptr).type(), typeid(const void*));
//    EXPECT_NO_THROW(pr->set(P::Param6Ptr, static_cast<void*>(&pr + 11)));
//    EXPECT_EQ(pr->get(P::Param6Ptr).type(), typeid(const void*));

//    // a few itertions
//    for (int i = 1; i <= 10; i++) {
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param1Bool, i % 2 == 0));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param2Float, 1. / i));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param3Int, i));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param4Bool, false));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param5Str, "new value"));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param6Ptr, static_cast<void*>(&pr + i)));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param7UInt, i * i));
//    }

//    EXPECT_EQ(pr->get(P::Param7UInt).as<int>(), -1); // not dispatched
//    EXPECT_NO_THROW(bus.dispatch());
//    EXPECT_EQ(pr->get(P::Param7UInt).as<int>(), -1); // still not dispatched
//    EXPECT_NO_THROW(bus.realtimeDispatch());
//    EXPECT_NE(pr->get(P::Param7UInt).as<int>(), -1); // some dispatched

//    if (10 * 7 > SOUND_PROCESSOR_BUS_SIZE) {
//        for (int i = 0; i < 10 * 7 / SOUND_PROCESSOR_BUS_SIZE; i++) {
//            EXPECT_NO_THROW(bus.dispatch());
//            EXPECT_NO_THROW(bus.realtimeDispatch());
//        }
//    }

//    EXPECT_EQ(pr->get(P::Param1Bool).as<bool>(), true);
//    EXPECT_EQ(pr->get(P::Param2Float).as<float>(), .1f);
//    EXPECT_EQ(pr->get(P::Param2Float).as<double>(), .1f);
//    EXPECT_EQ(pr->get(P::Param3Int).as<int>(), 10);
//    EXPECT_EQ(pr->get(P::Param4Bool).as<bool>(), false);
//    EXPECT_STREQ(pr->get(P::Param5Str).as<const char*>(), "new value");
//    EXPECT_EQ(pr->get(P::Param6Ptr).as<const void*>(), &pr + 10);
//    EXPECT_EQ(pr->get(P::Param7UInt).as<unsigned>(), 100);
//}

//TEST(SoundProcessorMessageBusTest, parametersStress)
//{
//    typedef Parameter::TestProcessor P;

//    MessageBus bus;

//    auto pr = std::make_shared<TProcessor>();
//    bus.addProcessor(pr.get());

//    unsigned count = SOUND_PROCESSOR_BUS_SIZE * (SOUND_PROCESSOR_BUS_HOLD_FACTOR + 1); // * number of params

//    for (unsigned int i = 1; i <= count; i++) {
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param1Bool, i % 2 == 0));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param2Float, 1. / i));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param3Int, i));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param4Bool, false));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param5Str, "new value"));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param6Ptr, static_cast<void*>(&pr + i)));
//        EXPECT_NO_THROW(bus.sendParameter(pr->id(), P::Param7UInt, i * i));
//    }

//    EXPECT_NE(pr->get(P::Param7UInt).as<int>(), -1); // was stress offload

//    for (unsigned int i = 0; i < SOUND_PROCESSOR_BUS_HOLD_FACTOR; i++) {
//        EXPECT_NO_THROW(bus.dispatch());
//        EXPECT_NO_THROW(bus.realtimeDispatch());
//    }

//    EXPECT_EQ(pr->get(P::Param1Bool).as<bool>(), true);
//    EXPECT_EQ(pr->get(P::Param2Float).as<float>(), 1.f / count);
//    ;
//    EXPECT_EQ(pr->get(P::Param3Int).as<int>(), count);
//    EXPECT_EQ(pr->get(P::Param4Bool).as<bool>(), false);
//    EXPECT_STREQ(pr->get(P::Param5Str).as<const char*>(), "new value");
//    EXPECT_EQ(pr->get(P::Param6Ptr).as<const void*>(), &pr + count);
//    EXPECT_EQ(pr->get(P::Param7UInt).as<unsigned>(), count * count);
//}

//TEST(SoundProcessorMessageBusTest, incomingSignals)
//{
//    typedef Parameter::TestProcessor P;

//    MessageBus bus;

//    auto pr = std::make_shared<TProcessor>();
//    bus.addProcessor(pr.get());

//    RuntimeInfo runtime;
//    runtime.bus = &bus;

//    EXPECT_NO_THROW(bus.sendParameter(pr->id(), Parameter::Processor::Runtime, &runtime));
//    EXPECT_NO_THROW(bus.realtimeDispatch());

//    EXPECT_EQ(pr->get(Parameter::Processor::Runtime).as<const RuntimeInfo*>(), &runtime);

//    bus.addWatcher([](unsigned, Signal::ID signal, Value value) {
//        EXPECT_TRUE(false);
//    }, pr->id());

//    for (int i = 1; i < SOUND_PROCESSOR_BUS_SIZE; i++) {
//        bus.sendCommand(pr->id(), Command::TestProcessor::EmitSignal);
//    }

//    EXPECT_NO_THROW(bus.dispatch());

//    //    EXPECT_EQ(pr->get(P::Param2Float).as<float>(), 0.4f);
//    //    EXPECT_EQ(pr->get(P::Param3Int).as<int>(), -135);
//    //    EXPECT_EQ(pr->get(P::Param4Bool).as<bool>(), true);
//    //    EXPECT_STREQ(pr->get(P::Param5Str).as<const char*>(), "test string");
//    //    EXPECT_EQ(pr->get(P::Param6Ptr).as<const void*>(), nullptr);
//    //    EXPECT_EQ(pr->get(P::Param7UInt).as<unsigned>(), -1);

//    //    EXPECT_EQ(pr->get(P::Param6Ptr).type(), typeid(const void*));
//    //    EXPECT_NO_THROW(pr->set(P::Param6Ptr, static_cast<void*>(&pr + 11)));
//    //    EXPECT_EQ(pr->get(P::Param6Ptr).type(), typeid(const void*));

//    //    // a few itertions
//    //    for (int i = 1; i <= 10; i++) {
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param1Bool, i % 2 == 0));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param2Float, 1. / i));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param3Int, i));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param4Bool, false));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param5Str, "new value"));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param6Ptr, static_cast<void*>(&pr + i)));
//    //        EXPECT_NO_THROW(bus.passParameter(pr->id(), P::Param7UInt, i * i));
//    //    }

//    //    EXPECT_EQ(pr->get(P::Param7UInt).as<int>(), -1); // not dispatched
//    //    EXPECT_NO_THROW(bus.dispatch());
//    //    EXPECT_EQ(pr->get(P::Param7UInt).as<int>(), -1); // still not dispatched
//    //    EXPECT_NO_THROW(bus.realtimeDispatch());
//    //    EXPECT_NE(pr->get(P::Param7UInt).as<int>(), -1); // some dispatched

//    //    if (10 * 7 > SOUND_PROCESSOR_BUS_SIZE) {
//    //        for (int i = 0; i < 10 * 7 / SOUND_PROCESSOR_BUS_SIZE; i++) {
//    //            EXPECT_NO_THROW(bus.dispatch());
//    //            EXPECT_NO_THROW(bus.realtimeDispatch());
//    //        }
//    //    }

//    //    EXPECT_EQ(pr->get(P::Param1Bool).as<bool>(), true);
//    //    EXPECT_EQ(pr->get(P::Param2Float).as<float>(), .1f);
//    //    EXPECT_EQ(pr->get(P::Param2Float).as<double>(), .1f);
//    //    EXPECT_EQ(pr->get(P::Param3Int).as<int>(), 10);
//    //    EXPECT_EQ(pr->get(P::Param4Bool).as<bool>(), false);
//    //    EXPECT_STREQ(pr->get(P::Param5Str).as<const char*>(), "new value");
//    //    EXPECT_EQ(pr->get(P::Param6Ptr).as<const void*>(), &pr + 10);
//    //    EXPECT_EQ(pr->get(P::Param7UInt).as<unsigned>(), 100);
//}
