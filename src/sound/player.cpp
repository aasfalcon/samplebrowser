#include <functional>
#include <mutex>

#include "inputstream.h"
#include "player.h"

using namespace Sound;

template <typename T>
Player<T>::Player()
{
}

template <typename T>
Player<T>::~Player()
{
}

template <typename T>
void Player<T>::play(const std::string& path)
{
    _stream = std::make_shared<InputStream>(path);
    BasicStream::Info info;
    *_stream >> info;
    _readBuffer.reallocate(info.channels, this->feedFrames(info.sampleRate));

    this->start(info.channels, info.sampleRate,
        [this](ConstFrame<T>& begRef, ConstFrame<T>& endRef) -> bool {
            return feed(begRef, endRef);
        });
}

template <typename T>
bool Player<T>::feed(ConstFrame<T>& begRef, ConstFrame<T>& endRef)
{
    bool isStopping = false;

    try {
        *_stream >> _readBuffer;
    } catch (BasicStream::Eof eof) {
        isStopping = true;
        auto silenceBegin = _readBuffer.begin() + int(eof.tail());
        _readBuffer.silence(silenceBegin, _readBuffer.end());
    }

    begRef = _readBuffer.cbegin();
    endRef = _readBuffer.cend();
    return isStopping;
}

SOUND_INSTANTIATE(Player);
