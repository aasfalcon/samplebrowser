#include "meter.h"
#include "constframe.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Meter<T>::Meter()
{
    this->set(Property::Meter::Peaks_Sound_Processor_PeaksFrame, _peaks.cbegin());
}

template <typename T>
Meter<T>::~Meter()
{
}

template <typename T>
void Meter<T>::commandInit()
{
    Processor<T>::commandInit();
    _peaks.reallocate(this->buffer().channels(), 1);
}

template <typename T>
void Meter<T>::process()
{
    auto& buffer = this->buffer();
    Frame<double> peaksFrame = _peaks.begin();
    unsigned channels = buffer.channels();

    _peaks.silence();

    for (auto frame = buffer.begin(); frame != buffer.end(); ++frame) {
        for (unsigned i = 0; i < channels; i++) {
            T value = frame.at(i);

            if (value < 0) {
                value = -value;
            }

            Sample<double> sample = value;

            if (double(sample) > double(peaksFrame[i])) {
                peaksFrame[i] = sample;
            }
        }
    }
}

SOUND_INSTANTIATE(Sound::Processor::Meter);
