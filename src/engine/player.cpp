#include <QStringList>

#include "config.h"
#include "player.h"

Player::Player(Client &client)
    : QtJack::Processor(client)
    , _client(client)
{
    this->_preparing = false;
    this->_stopping = false;
    this->_processing = false;
    this->_position = 0;
    this->_buffer = 0;
    this->_level = this->_config.value("player/level",
                                       WEE_PLAYER_LEVEL_DEFAULT).toFloat();
    this->_client.setMainProcessor(this);
}

Player::~Player() {
    if (this->_buffer) {
        delete this->_buffer;
    }
}

Client &Player::client() {
    return _client;
}

void Player::process(int frames) REALTIME_SAFE {
    this->_processing = true;
    QtJack::AudioBuffer lob = this->_client.left().buffer(frames);
    QtJack::AudioBuffer rob = this->_client.right().buffer(frames);

    lob.clear();
    rob.clear();

    if (this->_preparing || this->_position >= this->_file.frames()) {
        this->_processing = false;
        return;
    }

    int max_frames = frames;

    if (this->_position + frames >= this->_file.frames()) {
        max_frames = this->_file.frames() - this->_position;
    }

    int channels = this->_file.channels();

    for (int i = 0; i < max_frames; i++) {
        float left = this->_buffer[this->_position * channels];
        lob.write(i, left);

        if (channels > 1) {
            float right = this->_buffer[this->_position * channels + 1];
            rob.write(i, right);
        }

        this->_position ++;
    }

    lob.multiply(this->_level);

    if (channels == 1) {
        lob.copyTo(rob);
    } else {
        rob.multiply(this->_level);
    }

    this->_processing = false;
}

float Player::level() const {
    return this->_level;
}

void Player::setLevel(float value) {
    this->_level = value;
    this->_config.setValue("player/level", value);
}

void Player::run() {
    while (!this->_stopping) {
        if (!this->_filename.isEmpty() && !this->_processing) {
            this->_preparing = true;
            this->_file.read(this->_filename);
            this->_file.resample(this->_client.sampleRate());

            int size = this->_file.frames() * this->_file.channels();

            if (this->_buffer) {
                delete this->_buffer;
            }

            this->_buffer = new SoundFile::Sample[size];

            memcpy(this->_buffer,
                   this->_file.buffer().data(),
                   size * sizeof(float));

            this->_position = 0;
            this->_filename.clear();
            this->_preparing = false;
        } else {
            this->usleep(10);
        }
    }
}

void Player::play(const QString &filename) {
    this->_filename = filename;
}

void Player::stop() {
    this->_stopping = true;
}
