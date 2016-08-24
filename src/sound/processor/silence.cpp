#define PROCESSOR Silence

#include "silence.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
void Silence<T>::process()
{
    auto& out = this->output();
    out.silence();
}

INSTANTIATE;
