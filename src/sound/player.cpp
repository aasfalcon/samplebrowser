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
    BasicStream::Info info;
    _stream = std::make_shared<InputStream>(path);
    *_stream >> info;

    auto feed = [](RingBuffer<Float32>& ring, bool& isEnough, void* ptr) {
        auto player = reinterpret_cast<Player<T>*>(ptr);

        while (!ring.isFull()) {
            bool isEof = player->fillBuffer(player->_readBuffer);

            player->_mutex.lock();
            ring.push(player->_readBuffer.cbegin(), player->_readBuffer.cend());
            isEnough = isEof;
            player->_mutex.unlock();

            if (isEof) {
                break;
            }
        }
    };

    _readBuffer.reallocate(info.channels, this->feedFrames(info.sampleRate));
    this->start(info.channels, info.sampleRate, feed, this);
}

template <typename T>
bool Player<T>::fillBuffer(Buffer<Float32> &buffer)
{
    bool stop = false;

    try {
        *_stream >> buffer;
    } catch (BasicStream::Eof eof) {
        stop = true;
        auto silenceBegin = buffer.begin() + int(eof.tail());
        buffer.silence(silenceBegin, buffer.end());
    }

    return stop;
}

SOUND_INSTANTIATE(Player);
