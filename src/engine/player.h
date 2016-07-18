#ifndef PLAYER_H
#define PLAYER_H

#include <QSettings>
#include <QThread>

#include "qtjack/processor.h"

#include "client.h"
#include "soundfile.h"

class Player : public QtJack::Processor, public QThread
{
	public:
        Player(Client &client);
		~Player();
        float level() const;
        void setLevel(float value);
        void play(const QString &filename);
        void run();
        void stop();
        Client &client();

    protected:
        virtual void process(int samples) REALTIME_SAFE;
	
    private:
        SoundFile::Sample *_buffer;
        Client &_client;
        QSettings _config;
        SoundFile _file;
        QString _filename;
        float _level;
        bool _stopping;
        bool _preparing;
        bool _processing;
        int _position;
};

#endif // PLAYER_H
