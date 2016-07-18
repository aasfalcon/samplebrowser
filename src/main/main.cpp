#include "mainwindow.h"
#include "engine/config.h"
#include "engine/player.h"
#include <QApplication>

class WExplorer: public QApplication
{
private:
    Client _client;
    Player _player;
    MainWindow _window;

public:
    WExplorer(int argc, char *argv[])
        : QApplication(argc, argv)
        , _client()
        , _player(this->_client)
        , _window(0, _player)
    {
        // for QSettings
        QCoreApplication::setOrganizationName(QString(WEE_APPLICATION_NAME) + " Development");
        QCoreApplication::setOrganizationDomain("wavexplorer.org");
        QCoreApplication::setApplicationName(WEE_APPLICATION_NAME);
    }

    int exec() {
        this->_client.connect();
        this->_player.start();

        this->_window.show();
        int result = QApplication::exec();

        this->_player.stop();
        this->_player.wait();
        this->_client.disconnect();

        return result;
    }
};

// Entry point
int main(int argc, char *argv[]) {
    return WExplorer(argc, argv).exec();
}
