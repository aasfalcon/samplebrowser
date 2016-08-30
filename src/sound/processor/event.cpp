#include <cstring>

#include "event.h"

using namespace Sound;
using namespace Sound::Processor;

Event::Event() {}

Event::~Event() {}

Event& Event::operator=(const Event& that)
{
    memcpy(this, &that, sizeof that);
    return *this;
}
