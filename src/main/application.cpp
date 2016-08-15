#include <QFile>
#include <QDebug>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

#include "application.h"
#include "config.h"
#include "shared/server.h"

Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
    , _server(Server::instance())
    , _window(new Browser())
{
    _server->startup(pluginPath().toStdString());
    configurePluginServer();
    makeConfigPath();
    initResources();
}

Application::~Application()
{
    _server->shutdown();
}

int Application::run()
{
    int result = 1;

    try {
        this->_window->show();
        result = exec();
    } catch (std::exception e) {
        qDebug() << e.what();
    }

    return result;
}

void Application::initResources()
{
    Q_INIT_RESOURCE(config);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(themes);
    QFile qss(":/themes/default/main.qss");
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());
}

void Application::makeConfigPath()
{
    QCoreApplication::setOrganizationName(APPLICATION_TEAM);
    QCoreApplication::setOrganizationDomain(APPLICATION_TEAM_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QStringList configPaths = QStandardPaths::standardLocations(
                QStandardPaths::AppConfigLocation);
    QDir configDir(configPaths[0]);

    if (!configDir.exists()) {
        configDir.mkpath(configDir.path());
    }
}

QString Application::pluginPath()
{
    return APPLICATION_PLUGIN_PATH;
}

void Application::configurePluginServer()
{
    QMap<QString, QString> providers;
    providers["IResampler"] = "PluginResamplerLSR";
    providers["IAudioFile"] = "PluginAudiofile";

    QSettings settings;
    settings.beginGroup("plugins");
    auto keys = settings.childKeys();

    for (auto it = keys.begin(); it != keys.end(); ++it) {
        QString key = *it;
        providers[key] = settings.value(key).toString();
    }

    for (auto it = providers.begin(); it != providers.end(); ++it) {
        std::string interface = it.key().toStdString();
        std::string plugin = it.value().toStdString();
        _server->prefer(interface, plugin);
    }
}
