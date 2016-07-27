#include <QStringList>

#include "config.h"
#include "player.h"
#include "sound/frame.h"

Player::Player(Client &client)
    : QtJack::Processor(client)
    , _client(client)
    , _file(0)
    , _preparing(false)
    , _processing(false)
    , _position(0)
    , _resampler(Sound::ResamplerPrivate::QualityMedium,
                 client.bufferSize(), 2, client.sampleRate())
    , _stopping(false)
{
    _peak[0] = _peak[1] = 0;
    _level = _config.value("player/level",
                                       WEE_PLAYER_LEVEL_DEFAULT).toFloat();
    _client.setMainProcessor(this);
}

Player::~Player() {
    if (_file) {
        delete _file;
    }
}

Client &Player::client() {
    return _client;
}

Sample *Player::peak() {
    return _peak;
}

void Player::process(int frames) REALTIME_SAFE {
    _processing = true;
    QtJack::AudioBuffer lob = _client.left().buffer(frames);
    QtJack::AudioBuffer rob = _client.right().buffer(frames);

    lob.clear();
    rob.clear();
    _peak[0] = _peak[1] = 0;

    if (_preparing || !_file || _position >= _file->frames()) {
        _processing = false;
        return;
    }

    int max_frames = frames;

    if (_position + frames >= _file->frames()) {
        max_frames = _file->frames() - _position;
    }

    Sound::ReadFrame<Sample> frame;

//    if (_file->sampleRate() != _client.sampleRate()) {
//        int samplePosition = _position * _buffer.channels();
//        _position += _resampler.resample(
//                    _buffer.data() + samplePosition,
//                    _buffer.length() - samplePosition,
//                    _file->sampleRate());
//        frame = _resampler.frame();
//    } else {
//        _position += max_frames;
//        frame = _buffer.frame(_position);
//    }

    int samplePosition = _position * _buffer.channels();
    _position += _resampler.resample(
                _buffer.data() + samplePosition,
                _buffer.length() - samplePosition,
                _file->sampleRate());
    frame = _resampler.frame();

    for (int i = 0; i < max_frames; i++) {
        float left = frame.get(0);
        float right = frame.get(1);

        lob.write(i, left);
        rob.write(i, right);

        if (_peak[0] < left) {
            _peak[0] = left;
        }

        if (_peak[1] < right) {
            _peak[1] = right;
        }

        frame.next();
    }

    lob.multiply(_level);
    rob.multiply(_level);

    _peak[0] *= _level;
    _peak[1] *= _level;
    _processing = false;
}

float Player::level() const {
    return _level;
}

Sound::Format Player::format() const
{
    return _file->format();
}

void Player::setLevel(float value) {
    _level = value;
    _config.setValue("player/level", value);
}

void Player::run() {
    while (!_stopping) {
        if (!_filename.isEmpty() && !_processing) {
            _preparing = true;

            if (_file) {
                delete _file;
                _file = NULL;
            }

            _file = new Sound::Input(_filename);
            _file->read(_buffer, _file->frames());

            if (_buffer.channels() != 2) {
                _buffer = _buffer.stereo();
            }

            _position = 0;
            _filename.clear();
            _resampler.reset();
            _preparing = false;
        } else {
            usleep(25);
        }
    }
}

void Player::play(const QString &filename) {
    _filename = filename;
}

void Player::stop() {
    _stopping = true;
}
