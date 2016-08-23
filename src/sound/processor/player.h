#ifndef SOUND_PROCESSOR_PLAYER_H
#define SOUND_PROCESSOR_PLAYER_H

#include <memory>
#include <string>

#include "resampler.h"

namespace Sound {

class InputStream;

SOUND_PROCESSOR_COMMANDS(Player, Resampler,
    FastForward,
    Pause,
    Play,
    Reverse,
    Rewind,
    SkipToEnd,
    SkipToStart,
    Stop);

SOUND_PROCESSOR_PROPERTIES(Player, Resampler,
    Loop_bool,
    Repeat_bool,
    Path_std_string);

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

    SOUND_INSTANTIATION_DECLARE(Player);
}
}

#endif // SOUND_PROCESSOR_PLAYER_H
