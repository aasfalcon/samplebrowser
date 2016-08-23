#define PROCESSOR Silence

#include "silence.h"

template <typename T>
void Silence<T>::process()
{
    auto& buffer = this->buffer();
    buffer.silence();
}

INSTANTIATE;
