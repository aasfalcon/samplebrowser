#include "meter.h"
#include "constframe.h"
#include "config.h"

#define PROCESSOR Meter
#include "shortcuts.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Meter<T>::Meter()
    : _counter(0)
{
    INITIALIZE;
    PARAMETER(unsigned, PassFrames, SOUND_PROCESSOR_METER_PASS_FRAMES);
}

template <typename T>
Meter<T>::~Meter()
{
}

template <typename T>
void Meter<T>::commandInit()
{
    Processor<T>::commandInit();
    auto& info = this->runtime();
    _peaksInput.reallocate(info.channelsInput, 1);
    _peaksInput.silence();
    _peaksInputExposed.reallocate(info.channelsInput, 1);
    _peaksInputExposed.silence();
    _peaksOutput.reallocate(info.channelsOutput, 1);
    _peaksOutput.silence();
    _peaksOutputExposed.reallocate(info.channelsOutput, 1);
    _peaksOutputExposed.silence();

    _peaksInputFrame = _peaksInputExposed.cbegin();
    _peaksOutputFrame = _peaksOutputExposed.cbegin();
}

template <typename T>
void Meter<T>::process()
{
    auto& info = this->runtime();
    unsigned passFrames = this->get(Parameter::Meter::PassFrames);
    unsigned counter = _counter;

    if (info.channelsOutput) {
        auto& out = this->output();
        Frame<T> peaksFrame = _peaksOutput.begin();

        for (auto frame = out.begin(); frame != out.end(); ++frame) {
            for (unsigned i = 0; i < info.channelsOutput; i++) {
                T value = frame.at(i).value();

                if (value  < 0) {
                    value  = -value ;
                }

                if (peaksFrame.at(i).value() < value ) {
                    peaksFrame[i] = T(value);
                }
            }

            if (++counter >= passFrames) {
                _peaksOutputExposed.copyFrom(_peaksOutput.cbegin(), _peaksOutput.cend());
                const auto *cpeaks = &_peaksOutputFrame;
                this->emitSignal(Signal::Meter::PeaksOutput, cpeaks);
                counter = 0;
            }
        }
    }

    if (info.channelsInput) {
        auto& in = this->input();
        Frame<T> peaksFrame = _peaksInput.begin();
        counter = _counter;

        for (auto frame = in.begin(); frame != in.end(); ++frame) {
            for (unsigned i = 0; i < info.channelsInput; i++) {
                T value = frame.at(i).value();

                if (value < 0) {
                    value = -value;
                }

                if (peaksFrame.at(i).value() < value) {
                    peaksFrame[i] = value;
                }
            }

            if (++counter >= passFrames) {
                _peaksInputExposed.copyFrom(_peaksInput.cbegin(), _peaksInput.cend());
                const auto *cpeaks = &_peaksInputFrame;
                this->emitSignal(Signal::Meter::PeaksInput, cpeaks);
                counter = 0;
            }
        }
    }

    _counter = counter;
}

INSTANTIATE;
