#include "browser.h"
#include "driverdialog.h"
#include "ui_browser.h"

Browser::Browser(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Browser)
{
    ui->setupUi(this);
}

Browser::~Browser()
{
    delete ui;
}

void Browser::on_actionSound_device_triggered()
{
    DriverDialog *dialog = new DriverDialog(this);
    dialog->exec();
}
