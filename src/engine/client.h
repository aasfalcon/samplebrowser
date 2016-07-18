#ifndef CLIENT_H
#define CLIENT_H

#include "qtjack/client.h"

class Client: public QtJack::Client
{
public:
    Client();
    void connect();
    void disconnect();
    inline QtJack::AudioPort &left() { return this->_left; }
    inline QtJack::AudioPort &right() { return this->_right; }

private:
    QtJack::AudioPort _left;
    QtJack::AudioPort _right;
};

#endif // CLIENT_H
