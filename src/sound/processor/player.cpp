#include <functional>
#include <mutex>

#define PROCESSOR Player

#include "inputstream.h"
#include "player.h"

template <typename T>
Player<T>::Player()
{
    COMMAND(Play);

    PROPERTY(bool, Loop, false);
    PROPERTY(bool, Repeat, false);
    PROPERTY(std::string, Path, "");
}

template <typename T>
Player<T>::~Player()
{
}

template <typename T>
void Player<T>::commandPlay()
{
    std::string path = this->get(Property::Player::Path);

    _stream = std::make_shared<InputStream>(path);
    BasicStream::Info info;
    *_stream >> info;
    _readBuffer.reallocate(info.channels, this->feedFrames(info.sampleRate));

    auto feed = [this](ConstFrame<T>& begRef, ConstFrame<T>& endRef) -> bool {
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
    };

    this->set(Property::Resampler::Feed, feed);
    this->set(Property::Resampler::SourceChannels, info.channels);
    this->set(Property::Resampler::SourceSampleRate, info.sampleRate);

    this->call(Command::Resampler::Start);
}

INSTANTIATE;
