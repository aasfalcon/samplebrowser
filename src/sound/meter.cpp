#include "meter.h"

#include "frame.h"
#include "constframe.h"
#include "sample.h"

template<typename T>
Meter<T>::Meter()
    : _peaksBuffer(1, 32)
{

}

template<typename T>
Meter<T>::~Meter()
{

}

template<typename T>
void Meter<T>::init()
{
    _peaksBuffer.reallocate(this->out().channels(), 1);
}

template<typename T>
const ConstFrame<T> Meter<T>::peaks() const
{
    return _peaksBuffer.cbegin();
}

template<typename T>
void Meter<T>::process()
{
    auto out = this->out();
    unsigned channels = out.channels();
    Frame<T> peak = _peaksBuffer.begin();

    _peaksBuffer.silence();

    for (auto frame = out.begin(); frame != out.end(); ++frame) {
        for (unsigned i = 0; i < channels; i++) {
            T sample = frame.at(i);
            T abs = sample >= 0 ? sample : -sample;

            if (abs > peak.at(i)) {
                peak.put(i, abs);
            }
        }
    }
}

SOUND_INSTANTIATE(Meter)
