#ifndef SOUND_PROCESSOR_METER_H
#define SOUND_PROCESSOR_METER_H

#include "buffer.h"
#include "processor.h"

namespace Sound {

SOUND_PROCESSOR_PROPERTIES(Meter, Processor,
    Peaks_Sound_Processor_PeaksFrame);

template <typename T>
class ConstFrame;

namespace Processor {
    typedef ConstFrame<double> PeaksFrame;

    template <typename T>
    class Meter : public Processor<T> {
    public:
        Meter();
        ~Meter() override;

    protected:
        void commandInit() override;

        void process() override;

    private:
        Buffer<double> _peaks;
    };

    SOUND_INSTANTIATION_DECLARE(Meter);
}
}

#endif // SOUND_PROCESSOR_METER_H
