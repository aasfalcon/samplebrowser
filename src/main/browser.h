#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>

namespace Ui {
class Browser;
}

class Browser : public QMainWindow {
    Q_OBJECT

public:
    explicit Browser(QWidget* parent = 0);
    ~Browser();

private slots:
    void on_actionSound_device_triggered();

private:
    Ui::Browser* ui;
};

#endif // BROWSER_H
