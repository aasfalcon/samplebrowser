#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QMap>
#include <QString>
#include <memory>

#include "browser.h"

class Server;

class Application : private QApplication {
public:
    Application(int argc, char* argv[]);
    ~Application();

    int run();

private:
    std::shared_ptr<Server> _server;
    std::unique_ptr<Browser> _window;

    void initConfig();
    void initLog();
    void initResources();
    QString pluginPath();
    void prefer(const QMap<QString, QString>& providers);
    void preparePluginServer();
    void selectTheme(const QString& theme);
};

#endif // APPLICATION_H
