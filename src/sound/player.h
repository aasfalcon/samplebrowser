#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

#include "inputstream.h"
#include "resampler.h"

template <typename T>
class Player : public Resampler<T> {
public:
    Player();
    ~Player();

    void play(const std::string& path);

private:
    Buffer<Sound::Float32> _readBuffer;
    std::shared_ptr<InputStream> _stream;
};

SOUND_INSTANTIATION_DECLARE(Player);

#endif // PLAYER_H
