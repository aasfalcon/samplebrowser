#include "client.h"

#include "config.h"
#include <QStringList>

Client::Client()
{
    if (!this->connectToServer(WEE_APPLICATION_NAME)) {
        qDebug() << "Error: unable to connect JACK server.";
        std::exit(1);
    }

    this->_left = this->registerAudioOutPort("left");
    this->_right = this->registerAudioOutPort("right");
}

void Client::connect() {
    this->activate();
    QStringList clients = this->clientList();

    if (clients.contains("system")) {
        QList<QtJack::Port> systemPorts = this->portsForClient("system");
        int leftIndex = -1;
        bool rightConnected = false;

        for (int i = 0; i < systemPorts.size(); i++) {
            QtJack::Port port = systemPorts[i];

            if (port.isAudioPort() && port.isInput()) {
                if (leftIndex < 0) {
                    QtJack::Client::connect(this->_left, port);
                    leftIndex = i;
                } else if (!rightConnected) {
                    QtJack::Client::connect(this->_right, port);
                    rightConnected = true;
                    break;
                }
            }
        }

        if (!rightConnected && leftIndex >= 0) {
            QtJack::Client::connect(this->_right, systemPorts[leftIndex]);
        }
    }
}

void Client::disconnect() {
    this->deactivate();

    QList<QtJack::Port> systemPorts = this->portsForClient("system");

    for (int i = 0; i < systemPorts.size(); i++) {
        QtJack::Port port = systemPorts[i];

        if (port.isConnectedTo(this->_left)) {
            QtJack::Client::disconnect(port, this->_left);
        }

        if (port.isConnectedTo(this->_right)) {
            QtJack::Client::disconnect(port, this->_right);
        }
    }
}
