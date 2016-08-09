#ifndef DRIVERDIALOG_H
#define DRIVERDIALOG_H

#include <memory>
#include <QAbstractButton>
#include <QDialog>
#include <QStatusBar>

#include "shared/idriver.h"
#include "sound/sound.h"

class Driver;
class QComboBox;

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
    void on_bufferCount_valueChanged(int value);
    void on_bufferSize_currentIndexChanged(int index);
    void on_driverType_currentIndexChanged(int index);
    void on_duplexMode_toggled(bool checked);
    void on_inChannels_valueChanged(int value);
    void on_inputDevice_currentIndexChanged(int index);
    void on_outChannels_valueChanged(int value);
    void on_outputDevice_currentIndexChanged(int index);
    void on_sampleFormat_currentIndexChanged(int index);
    void on_sampleRate_currentIndexChanged(int index);
    void on_xrunStop_toggled(bool checked);
    void on_buttonBox_clicked(QAbstractButton *button);
    void on_watcher_finished();

private:
    struct ApiSettings {
        unsigned inputChannels;
        unsigned inputDeviceId;
        bool isActive;
        bool isBreakOnXrun;
        bool isDuplex;
        unsigned outputChannels;
        unsigned outputDeviceId;
        Sound::Type sampleFormat;
        unsigned sampleRate;
        unsigned bufferSize;
        unsigned bufferCount;
    };

    static const int DEFAULT_CHANNEL_COUNT = 2;
    static const int DEFAULT_SAMPLE_RATE = 44100;
    static const Sound::Type DEFAULT_SAMPLE_FORMAT = Sound::TypeFloat32;
    static const bool DEFAULT_IS_DUPLEX = false;
    static const bool DEFAULT_IS_BREAK_ON_XRUN = false;
    static const int DEFAULT_BUFFER_SIZE = 256;
    static const unsigned DEFAULT_BUFFER_COUNT = 3;

    Ui::DriverDialog *ui;
    IDriver::ApiType _apiCurrent;
    unsigned _apiIndex;
    std::shared_ptr<Driver> _driver;
    ApiSettings _models[IDriver::ApiCount];
    ApiSettings *_model;
    bool _isUpdateLocked;
    QStatusBar *_status;

    void setApiType(IDriver::ApiType value);
    void settingsApply();
    void settingsLoad();
    void settingsSave();
    void populateBufferSizes();
    void populateDriverTypes();
    void populateInputs();
    void populateOutputs();
    void populateSampleRates();
    void populateSampleFormat();
    void selectByValue(QComboBox *target, unsigned value);
    void updateDisabled();
    Sound::Type selectFormat(unsigned outFlags, unsigned inFlags,
                             bool isDuplex, Sound::Type defaultFormat = DEFAULT_SAMPLE_FORMAT);
    unsigned selectSampleRate(const unsigned *outRates, unsigned outRateCount,
                              const unsigned *inRates, unsigned inRateCount,
                              bool isDuplex, unsigned defaultRate = DEFAULT_SAMPLE_RATE);
};

#endif // DRIVERDIALOG_H
