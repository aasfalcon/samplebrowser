#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <string>

#include "resampler.h"
#include "inputstream.h"

template<typename T>
class Player: public Resampler<T>
{
public:
    Player();
    ~Player();

    void play(const std::string &path);
    void process();
    void stop();

private:
    typedef std::shared_ptr<InputStream<T>> PStream;
    Buffer<T> _playerBuffer;
    InputStreamInfo _info;
    bool _isPlaying;
    PStream _stream;

    void calibrate();
};

#include "player.tcc"

#endif // PLAYER_H
