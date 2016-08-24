#define PROCESSOR Meter

#include "meter.h"
#include "constframe.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Meter<T>::Meter()
{
    PROPERTY(ConstFrame<double>, PeaksInput, _peaksInput.cbegin());
    PROPERTY(ConstFrame<double>, PeaksOutput, _peaksOutput.cbegin());
}

template <typename T>
Meter<T>::~Meter()
{
}

template <typename T>
void Meter<T>::commandInit()
{
    Processor<T>::commandInit();

    _peaksInput.reallocate(this->input().channels(), 1);
    _peaksInput.silence();
    this->setProperty(Property::Meter::PeaksInput, _peaksInput.cbegin());

    _peaksOutput.reallocate(this->output().channels(), 1);
    _peaksOutput.silence();
    this->setProperty(Property::Meter::PeaksOutput, _peaksOutput.cbegin());
}

template <typename T>
void Meter<T>::process()
{
    auto& info = this->runtime();

    if (info.channelsOutput) {
        auto& out = this->output();
        Frame<double> peaksFrame = _peaksOutput.begin();

        _peaksOutput.silence();

        for (auto frame = out.begin(); frame != out.end(); ++frame) {
            for (unsigned i = 0; i < info.channelsOutput; i++) {
                T value = frame.at(i);

                if (value < 0) {
                    value = -value;
                }

                Sample<double> sample = Sample<T>(value);

                if (double(sample) > double(peaksFrame[i])) {
                    peaksFrame[i] = sample;
                }
            }
        }
    }

    peaksFrame = _peaksInput.begin();
    _peaksInput.silence();
    _peaksOutput.silence();

}

INSTANTIATE;
