#ifndef SOUND_PROCESSOR_ROOT_H
#define SOUND_PROCESSOR_ROOT_H

#include "messagebus.h"
#include "processor.h"
#include "shared/ring.h"

namespace Sound {

namespace Processor {

    template <typename T>
    class Root : public Processor<T> {
    public:
        Root();
        ~Root();

    protected:
        void commandInit() override;
        Buffer<T> &input() override;
        void process() override;
        void processPost() override;
        void processPre() override;

    private:
        ConstFrame<T> _inputFrame;
        Buffer<T> _inputBuffer;
        Frame<T> _outputFrame;
    };

    SOUND_PROCESSOR_FACTORY(Root);
}
}

#endif // SOUND_PROCESSOR_ROOT_H
