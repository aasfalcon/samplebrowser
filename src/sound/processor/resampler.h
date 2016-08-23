#ifndef SOUND_PROCESSOR_RESAMPLER_H
#define SOUND_PROCESSOR_RESAMPLER_H

#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include "ringbuffer.h"
#include "ringfeeder.h"
#include "shared/iresampler.h"
#include "silence.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Resampler, Processor,
    Start,
    Stop);

SOUND_PROCESSOR_PROPERTIES(Resampler, Processor,
    Callback_Sound_Processor_Resampler_Feed,
    Library_Sound_Processor_ResamplerLibrary,
    Quality_IResampler_Quality,
    SourceChannels_unsigned,
    SourceSampleRate_unsigned);

namespace Processor {

    enum ResamplerLibrary {
        LibraryLibResample,
        LibraryLibSampleRate,
        LibrarySoXR,

        LibraryDefault = LibraryLibSampleRate,
    };

    template <typename T>
    class Resampler : public Silence<T> {
    public:
        typedef Float32 InternalFormat;
        typedef typename RingFeeder<T, InternalFormat>::FeedFunc Feed;

        Resampler();
        ~Resampler() override;

    protected:
        void commandInit() override;
        virtual void commandStart();
        virtual void commandStop();

        void process() override;
        unsigned feedFrames(unsigned sourceSampleRate);

    private:
        std::shared_ptr<RingFeeder<T, InternalFormat> > _feeder;
        std::mutex _mutex;
        std::shared_ptr<IResampler> _resampler;
        std::shared_ptr<RingBuffer<InternalFormat> > _ring;
    };

    SOUND_INSTANTIATION_DECLARE(Resampler);
}
}

#endif // SOUND_PROCESSOR_RESAMPLER_H
