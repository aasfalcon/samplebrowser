#include <functional>
#include <mutex>

#include "inputstream.h"
#include "player.h"

using namespace Sound;
using namespace Sound::Processor;

template <typename T>
Player<T>::Player()
{
    SOUND_REGISTER_COMMAND(Player, Play);
}

template <typename T>
Player<T>::~Player()
{
}

template <typename T>
void Player<T>::commandPlay()
{
    std::string path = this->get(Property::Player::Path_std_string);

    _stream = std::make_shared<InputStream>(path);
    BasicStream::Info info;
    *_stream >> info;
    _readBuffer.reallocate(info.channels, this->feedFrames(info.sampleRate));

    auto feedLambda = [this](ConstFrame<T>& begRef, ConstFrame<T>& endRef) -> bool {
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

    this->set(Property::Resampler::Callback_Sound_Processor_Resampler_Feed, feedLambda);
    this->set(Property::Resampler::SourceChannels_unsigned, info.channels);
    this->set(Property::Resampler::SourceSampleRate_unsigned, info.sampleRate);

    this->call(Command::Resampler::Start);
}

SOUND_INSTANTIATE(Sound::Processor::Player);
