#include "player.h"

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

    auto feed = [](RingBuffer<Sound::Float32>& ring, bool& isEnough, void* ptr) {
        auto player = reinterpret_cast<Player<T>*>(ptr);
        bool isEof = false;
        auto& buf = player->_readBuffer;

        while (!ring.isFull()) {
            try {
                *player->_stream >> buf;
            } catch (BasicStream::Eof eof) {
                isEof = true;
                buf.silence(buf.begin() + int(eof.tail()), buf.end());
            }

            player->_mutex.lock();
            ring.push(buf.cbegin(), buf.cend());
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

SOUND_INSTANTIATE(Player);
