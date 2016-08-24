#ifndef SOUND_PROCESSOR_METER_H
#define SOUND_PROCESSOR_METER_H

#include "buffer.h"
#include "processor.h"

namespace Sound {

SOUND_PROCESSOR_PARAMETERS(Meter, Processor,
    PassFrames // unsigned
    );

SOUND_PROCESSOR_SIGNALS(Meter, Processor,
    PeaksInput, // ConstFrame<double> *
    PeaksOutput // ConstFrame<double> *
    );

template <typename T>
class ConstFrame;

namespace Processor {
    template <typename T>
    class Meter : public Processor<T> {
    public:
        Meter();
        ~Meter() override;

    protected:
        void commandInit() override;

        void process() override;

    private:
        Buffer<T> _peaksInput;
        Buffer<double> _peaksInputExposed;
        Buffer<T> _peaksOutput;
        Buffer<double> _peaksOutputExposed;
        ConstFrame<double> _peaksInputFrame;
        ConstFrame<double> _peaksOutputFrame;
        unsigned _counter;
    };

    SOUND_PROCESSOR_FACTORY(Meter);
}
}

#endif // SOUND_PROCESSOR_METER_H
