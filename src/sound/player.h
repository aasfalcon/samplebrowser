#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

#include "resampler.h"

namespace Sound {

class InputStream;

template <typename T>
class Player : public Resampler<T> {
public:
    Player();
    ~Player();

    void play(const std::string& path);

protected:
    virtual bool feed(ConstFrame<T>& begRef, ConstFrame<T>& endRef);

private:
    Buffer<T> _readBuffer;
    std::shared_ptr<InputStream> _stream;
};

SOUND_INSTANTIATION_DECLARE(Player);
}

#endif // PLAYER_H
