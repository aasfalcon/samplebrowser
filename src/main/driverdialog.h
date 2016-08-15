#ifndef DRIVERDIALOG_H
#define DRIVERDIALOG_H

#include <memory>

#include <QDialog>
#include <QMap>

#include "shared/idriver.h"
#include "sound/sound.h"

class QAbstractButton;
class QComboBox;
class QLabel;
class QStatusBar;
class QString;

class Driver;

namespace Ui {
class DriverDialog;
}

class DriverDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DriverDialog(QWidget *parent = 0);
    ~DriverDialog();

private slots:
    void on_DriverDialog_accepted();
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_driverType_currentIndexChanged(int index);

private:
    static const int DEFAULT_CHANNEL_COUNT = 2;
    static const int DEFAULT_SAMPLE_RATE = 44100;
    static const IDriver::SampleFormat DEFAULT_SAMPLE_FORMAT = IDriver::SampleFloat32;
    static const bool DEFAULT_IS_DUPLEX = false;
    static const bool DEFAULT_IS_BREAK_ON_XRUN = false;
    static const bool DEFAULT_IS_REALTIME = true;
    static const unsigned DEFAULT_REALTIME_PRIORITY = 60;
    static const int DEFAULT_BUFFER_SIZE = 256;
    static const unsigned DEFAULT_BUFFER_COUNT = 3;

    Ui::DriverDialog *ui;
    std::shared_ptr<Driver> _driver;
    static QMap<IDriver::SampleFormat, QString> _formatMap;
    IDriver::ConnectOptions _model;
    bool _isUpdateLocked;
    double _latency;
    QLabel *_latencyText;
    QStatusBar *_statusBar;
    QLabel *_statusIcon;
    QLabel *_statusText;

    void modelApply();
    void modelBound();
    void modelInitialize(IDriver::ApiType apiType);
    void modelUpdate();
    void modelLoad();
    void modelSave();

    void controlsUpdate();
    void critical(const QString &caption, const QString &message,
                  const QString &source = QString());
    void populateBufferSizes();
    void populateDriverTypes();
    void populateInputs(IDriver::ApiType type);
    void populateOutputs(IDriver::ApiType type);
    void populateSampleRates(IDriver::ApiType type);
    void populateSampleFormats(IDriver::ApiType type);

    void selectByValue(QComboBox *target, unsigned value);
    void setStatus(const QString &themeIcon, const QString &message);
    IDriver::SampleFormat selectFormat(
            unsigned outFlags, unsigned inFlags, bool isDuplex,
            IDriver::SampleFormat defaultFormat = DEFAULT_SAMPLE_FORMAT);
    unsigned selectSampleRate(const unsigned *outRates, unsigned outRateCount,
                              const unsigned *inRates, unsigned inRateCount,
                              bool isDuplex, unsigned defaultRate = DEFAULT_SAMPLE_RATE);
    QString testFilePath();
    void updateStatus();
};

#endif // DRIVERDIALOG_H
