#include <stdexcept>

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QStandardPaths>

#include "browser.h"
#include "engine/config.h"
#include "plugins/shared/server.h"

class WExplorer: public QApplication
{
    Browser _window;

public:
    WExplorer(int argc, char *argv[]);
    int exec();
};

// Entry point
int main(int argc, char *argv[]) {
    auto server = Server::instance();
    server->startup("/tmp/wexplorer.build/src/plugins");
    int result = 1;

    server->prefer("IResampler", "PluginResamplerLSR");
    server->prefer("IAudioFile", "PluginAudiofileMPG123");

    try {
        WExplorer app(argc, argv);
        result = app.exec();
    } catch(...) {
        server->shutdown();
        throw;
    }

    return result;
}

WExplorer::WExplorer(int argc, char *argv[])
    : QApplication(argc, argv)
    , _window(0)
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
    setStyleSheet(qss.readAll());
}

int WExplorer::exec() {
    int result = 1;

    try {
        //this->_jackClient.connect();
        //this->_player.start();

        this->_window.show();
        result = QApplication::exec();
    } catch (std::exception e) {
        qDebug() << e.what();
    }

    return result;
}
