#include <memory>

#include <QFile>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

#include <log4cplus/consoleappender.h>
using namespace log4cplus;

#include "application.h"
#include "config.h"
#include "shared/log.h"
#include "shared/server.h"

Application::Application(int argc, char *argv[])
    : QApplication(argc, argv)
    , _server(Server::instance())
    , _window(new Browser())
{
    initLog();
    _server->startup(pluginPath().toStdString());
    preparePluginServer();
    initResources();
    initConfig();

    QSettings settings;
    selectTheme(settings.value("main/theme").toString());
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
        LOG(FATAL, e.what());
    }

    return result;
}

void Application::initConfig()
{
    QCoreApplication::setOrganizationName(APPLICATION_TEAM);
    QCoreApplication::setOrganizationDomain(APPLICATION_TEAM_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);

    QSettings defaults(":/config/default.conf", QSettings::IniFormat);
    QSettings settings;

    auto keys = defaults.allKeys();

    for (auto it = keys.begin(); it != keys.end(); ++it) {
        QString key = *it;

        if (!settings.contains(key)) {
            settings.setValue(key, defaults.value(key));
        }
    }

    settings.sync();
}

void Application::initLog()
{
#define LOG_PATTERN "%-5p *** %m%n[ %b:%L ] %M%n"

    auto console = SharedAppenderPtr(new ConsoleAppender(true, true));
    auto layout = std::auto_ptr<Layout>(new PatternLayout(LOG_PATTERN));
    auto logger = Logger::getInstance(APPLICATION_NAME);
    console->setLayout(layout);
    logger.addAppender(console);
    logger.setLogLevel(TRACE_LOG_LEVEL);
    LOG(TRACE, "Logger initialized");
}

void Application::initResources()
{
    Q_INIT_RESOURCE(config);
    Q_INIT_RESOURCE(icons);
    Q_INIT_RESOURCE(themes);
}

QString Application::pluginPath()
{
    return APPLICATION_PLUGIN_PATH;
}

void Application::preparePluginServer()
{
    QSettings settings;
    settings.beginGroup("plugins");
    auto keys = settings.childKeys();

    for (auto it = keys.begin(); it != keys.end(); ++it) {
        std::string interface = it->toStdString();
        std::string plugin = settings.value(*it).toString().toStdString();
        _server->prefer(interface, plugin);
    }
}

void Application::selectTheme(const QString &theme)
{
    QFile qss(QString(":/themes/%1/main.qss").arg(theme));
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());
}
