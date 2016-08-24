#ifndef SOUND_PROCESSOR_PLAYER_H
#define SOUND_PROCESSOR_PLAYER_H

#include <memory>
#include <string>

#include "resampler.h"

namespace Sound {

SOUND_PROCESSOR_COMMANDS(Player, Resampler,
    FastForward,
    Pause,
    Play,
    Reverse,
    Rewind,
    SkipToEnd,
    SkipToStart,
    Stop);

SOUND_PROCESSOR_PARAMETERS(Player, Processor,
    Loop, // bool
    Repeat // bool
    );

SOUND_PROCESSOR_PROPERTIES(Player, Resampler,
    Path // const char *
    );

SOUND_PROCESSOR_SIGNALS(Player, Resampler,
    EndOfFile, // bool
    Position // double
    )

class InputStream;

namespace Processor {

    template <typename T>
    class Player : public Resampler<T> {
    public:
        Player();
        ~Player() override;

    protected:
        virtual void commandPlay();

    private:
        Buffer<T> _readBuffer;
        std::shared_ptr<InputStream> _stream;
    };

    SOUND_PROCESSOR_FACTORY(Player);
}
}

#endif // SOUND_PROCESSOR_PLAYER_H
