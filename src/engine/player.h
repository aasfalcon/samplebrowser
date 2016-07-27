#ifndef PLAYER_H
#define PLAYER_H

#include <QSettings>
#include <QThread>

#include "qtjack/processor.h"

#include "client.h"
#include "config.h"
#include "sound/input.h"
#include "sound/realtimeresampler.h"

class Player : public QtJack::Processor, public QThread
{
	public:
        Player(Client &client);
		~Player();
        float level() const;
        Sound::Format format() const;
        void setLevel(float value);
        Sample *peak();
        void play(const QString &filename);
        void run();
        void stop();
        Client &client();

    protected:
        virtual void process(int samples) REALTIME_SAFE;
	
    private:
        Sound::Buffer<Sample> _buffer;
        Sample _peak[2];
        Client &_client;
        QSettings _config;
        Sound::Input *_file;
        QString _filename;
        float _level;
        bool _preparing;
        bool _processing;
        int _position;
        Sound::RealtimeResampler<Sample> _resampler;
        bool _stopping;
};

#endif // PLAYER_H
