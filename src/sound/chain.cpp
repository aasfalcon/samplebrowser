#include "chain.h"

using namespace Sound;

template <typename T>
Chain<T>::Chain()
{
}

template <typename T>
Chain<T>::~Chain()
{
}

template <typename T>
void Chain<T>::init()
{
    for (auto it = this->begin(); it != this->end(); ++it) {
        (*it)->kickIn(this->out(), this->in(),
            this->latency(), this->sampleRate());
    }
}

SOUND_INSTANTIATE(Chain)
