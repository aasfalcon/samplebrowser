#include <gtest/gtest.h>

#include "sound/processor/messagebus.h"

using namespace Sound;
using namespace Sound::Processor;

TEST(SoundProcessorMessageBusTest, ctor)
{
    EXPECT_NO_THROW(MessageBus bus);

    MessageBus bus;
    bus.clear();
}
