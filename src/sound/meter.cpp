#include "constframe.h"
#include "meter.h"

using namespace Sound;

template <typename T>
Meter<T>::Meter()
{
}

template <typename T>
Meter<T>::~Meter()
{
}

template <typename T>
void Meter<T>::init()
{
    _peaks.reallocate(this->out().channels(), 1);
}

template <typename T>
ConstFrame<T> Meter<T>::peaks() const
{
    return _peaks.cbegin();
}

template <typename T>
void Meter<T>::process()
{
    auto& out = this->out();
    Frame<T> peak = _peaks.begin();

    _peaks.silence();

    for (auto frame = out.begin(); frame != out.end(); ++frame) {
        for (unsigned i = 0; i < out.channels(); i++) {
            T sample = frame.at(i);

            if (sample < 0) {
                sample = -sample;
            }

            if (sample > peak[i]) {
                peak[i] = sample;
            }
        }
    }
}

SOUND_INSTANTIATE(Meter);
