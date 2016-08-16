#include "sample.h"
#include "player.h"

template<typename T>
Player<T>::Player()
    : Resampler<T> ()
    , _isPlaying(false)
{

}

template<typename T>
Player<T>::~Player()
{

}

template<typename T>
void Player<T>::calibrate()
{
    unsigned tail = _stream->bufferSize() % this->_bufferFrames;

    if (tail) {
        unsigned alignedBufferSize = _stream->bufferSize() - tail;
        _playerBuffer.resize(alignedBufferSize);
        _stream->setBufferSize(alignedBufferSize);
        _stream->seek(-int(tail), IAudioFile::SeekCur);
    }
}

template<typename T>
void Player<T>::stop()
{
    this->drop();
    _isPlaying = false;
}

template<typename T>
void Player<T>::play(const std::string &path)
{
    this->stop();

    _stream = PStream(new InputStream<T>(path));
    *_stream >> _info >> _playerBuffer;

    if (!_playerBuffer.isEmpty()) {
        this->feed(_playerBuffer, _info.sampleRate);
        _isPlaying = true;
        calibrate();
    }
}

template<typename T>
void Player<T>::process()
{
    if (_isPlaying) {
        if (this->_position >= this->_sourceFrames) {
            if (!_stream->eof()) {
                *_stream >> _playerBuffer;
                this->_position = 0;
            } else {
                _isPlaying = false;
                return;
            }
        }

        Resampler<T>::process();
    }
}

SOUND_INSTANTIATE(Player)
