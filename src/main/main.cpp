#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "mainwindow.h"
#include "engine/config.h"
#include "engine/player.h"
#include "plugins/bootstrap.h"

class WExplorer: public QApplication
{
public:
    WExplorer(int argc, char *argv[])
        : QApplication(argc, argv)
        , _client()
        , _player(this->_client)
        , _pluginServer(Bootstrap::initialize())
        , _window(0, _player)
    {
        // for QSettings
        QCoreApplication::setOrganizationName("WEx");
        QCoreApplication::setOrganizationDomain("wex.org");
        QCoreApplication::setApplicationName(WEE_APPLICATION_NAME);

        QString configPath = QStandardPaths::standardLocations(
                    QStandardPaths::AppConfigLocation)[0];
        QDir configDir(configPath);

        if (!configDir.exists()) {
            configDir.mkpath(configDir.path());
        }

        // link resources, load stylesheet
        Q_INIT_RESOURCE(resources);
        QFile qss(":/qss/main.qss");
        qss.open(QFile::ReadOnly);
        this->setStyleSheet(qss.readAll());
    }

    int exec() {
        int result = 1;

        try {
            this->_client.connect();
            this->_player.start();

            this->_window.show();
            result = QApplication::exec();
        } catch (Sound::Error e) {
            qDebug() << e.toStrinng();
        }

        this->_player.stop();
        this->_player.wait();
        this->_client.disconnect();

        Bootstrap::shutdown();
        _pluginServer = NULL;
        return result;
    }

private:
    Client _client;
    Player _player;
    IServerProvider *_pluginServer;
    MainWindow _window;

};

// Entry point
int main(int argc, char *argv[]) {
    return WExplorer(argc, argv).exec();
}
