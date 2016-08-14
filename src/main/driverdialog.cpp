#include <algorithm>
#include <list>
#include <string>

#include <RtAudio.h>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QStatusBar>
#include <QPushButton>
#include <QWidgetList>
#include <QMessageBox>

#include "driverdialog.h"
#include "ui_driverdialog.h"

#include "sound/driver.h"
#include "sound/player.h"
#include "engine/config.h"

QMap<IDriver::SampleFormat, QString> DriverDialog::_formatMap = {
    { IDriver::SampleInt8, tr("8 bit") },
    { IDriver::SampleInt16, tr("16 bit") },
    { IDriver::SampleInt24, tr("24 bit") },
    { IDriver::SampleInt32, tr("32 bit") },
    { IDriver::SampleFloat32, tr("Float 32 bit") },
    { IDriver::SampleFloat64, tr("Float 64 bit") },
};

DriverDialog::DriverDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DriverDialog)
    , _driver(new Driver)
    , _isUpdateLocked(false)
    , _latencyText(new QLabel(this))
    , _statusBar(new QStatusBar(this))
    , _statusIcon(new QLabel(this))
    , _statusText(new QLabel(this))
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Apply)->setText(tr("Test"));
    ui->statusLayout->addWidget(_statusBar);

    _statusBar->addWidget(_statusIcon);
    _statusBar->addWidget(_statusText);
    _statusBar->addPermanentWidget(_latencyText);
    _statusBar->setSizeGripEnabled(false);

    auto checkToggled = &QCheckBox::toggled;
    auto spinChanged = static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged);
    auto comboChanged = static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged);
    auto updateAll = [this]() { modelUpdate(); controlsUpdate(); };

    connect(ui->bufferCount,    spinChanged,    this, &DriverDialog::modelUpdate);
    connect(ui->bufferSize,     comboChanged,   this, &DriverDialog::modelUpdate);
    connect(ui->inChannels,     spinChanged,    this, &DriverDialog::modelUpdate);
    connect(ui->outChannels,    spinChanged,    this, &DriverDialog::modelUpdate);
    connect(ui->sampleFormat,   comboChanged,   this, &DriverDialog::modelUpdate);
    connect(ui->sampleRate,     comboChanged,   this, &DriverDialog::modelUpdate);
    connect(ui->xrunStop,       checkToggled,   this, &DriverDialog::modelUpdate);
    connect(ui->priority,       spinChanged,    this, &DriverDialog::modelUpdate);

    connect(ui->driverType,     comboChanged,   this, updateAll);
    connect(ui->duplexMode,     checkToggled,   this, updateAll);
    connect(ui->inputDevice,    comboChanged,   this, updateAll);
    connect(ui->outputDevice,   comboChanged,   this, updateAll);
    connect(ui->realtime,       checkToggled,   this, updateAll);

    modelLoad();
}

DriverDialog::~DriverDialog()
{
    delete ui;
}

void DriverDialog::controlsUpdate()
{
    if (!_isUpdateLocked) {
        _isUpdateLocked = true;

        populateDriverTypes();
        selectByValue(ui->driverType, _model.apiType);

        populateOutputs(_model.apiType);
        selectByValue(ui->outputDevice, _model.outputDeviceId);

        ui->duplexMode->setChecked(_model.isDuplex);
        populateInputs(_model.apiType);
        selectByValue(ui->inputDevice, _model.inputDeviceId);

        populateSampleFormats(_model.apiType);
        populateSampleRates(_model.apiType);
        populateBufferSizes();
        selectByValue(ui->sampleFormat, _model.sampleFormat);
        selectByValue(ui->sampleRate, _model.sampleRate);
        selectByValue(ui->bufferSize, _model.bufferSize);

        const IDriver::Api &api = _driver->apiInfo(_model.apiType);
        unsigned channelsInputMax = 0;
        unsigned channelsOutputMax = 0;

        if (api.deviceCount) {
            auto &inputDevice = api.devices[_model.inputDeviceId];
            auto &outputDevice = api.devices[_model.outputDeviceId];

            channelsInputMax = std::max(inputDevice.channelsInput,
                                     inputDevice.channelsDuplex);
            channelsOutputMax = std::max(outputDevice.channelsOutput,
                                      outputDevice.channelsDuplex);
        }

        ui->inChannels->setMaximum(int(channelsInputMax));
        ui->inChannels->setMinimum(0);
        ui->inChannels->setValue(int(_model.inputChannels));

        ui->outChannels->setMaximum(int(channelsOutputMax));
        ui->outChannels->setMinimum(0);
        ui->outChannels->setValue(int(_model.outputChannels));

        bool hasInput = channelsInputMax > 0;
        bool hasOutput = channelsOutputMax > 0;

        ui->bufferCount->setValue(int(_model.bufferCount));
        ui->bufferCount->setEnabled(hasInput || hasOutput);

        ui->realtime->setChecked(_model.isRealtime);
        ui->priority->setValue(_model.priority);

        ui->bufferSize->setEnabled(hasInput || hasOutput);
        ui->buttonBox->button(QDialogButtonBox::Apply)->setEnabled(hasOutput);

        ui->sampleFormat->setEnabled(ui->sampleFormat->count() > 1);
        ui->sampleRate->setEnabled(ui->sampleRate->count() > 1);

        if (hasInput || hasOutput) {
            bool isRealtime = ui->realtime->isChecked();
            ui->priority->setEnabled(isRealtime);
        } else {
            ui->realtime->setEnabled(false);
            ui->priority->setEnabled(false);
        }

        _isUpdateLocked = false;
    }
}

QString DriverDialog::testFilePath()
{
    QString filename = "testfile.ogg";
    QString result = QStandardPaths::locate(
                QStandardPaths::DataLocation, filename);

    if (result.isEmpty()) {
        QMessageBox *messageBox = new QMessageBox(this);
        messageBox->setIcon(QMessageBox::Critical);
        messageBox->setText(tr("Unable to find test sound file "
                               "\"%1\"").arg(filename));
        messageBox->setWindowTitle(tr("Settings test error"));
        messageBox->setStandardButtons(QMessageBox::Ok);
        messageBox->exec();
        delete messageBox;
        qDebug() << messageBox->text();
    }

    return result;
}

void DriverDialog::modelApply()
{
    setStatus("audio-headphones", tr("Checking current settings..."));
    try {
        _driver->connect(_model);
        _latency = double(_driver->latency()) * 1000 / _model.sampleRate;
        std::shared_ptr<Player<float>> player;
        auto processor = std::static_pointer_cast<Processor<Sound::Float32>>(player);
        _driver->setRoot(processor);
        player->play(testFilePath().toStdString());
        //_driver->disconnect();
        _model.isTested = true;
        updateStatus();
    } catch(std::runtime_error e) {
        setStatus("dialog-error", tr("Driver internal error"));
        qDebug() << e.what();
    } catch(RtAudioError e) {
        setStatus("dialog-error", tr("Driver error"));

        QMessageBox *messageBox = new QMessageBox(this);
        std::string message(e.what());
        std::string::size_type spacePos = message.find(' ');

        if (spacePos != std::string::npos) {
            message = "<b>Driver</b>:" + message.substr(spacePos);
        }

        messageBox->setIcon(QMessageBox::Critical);
        messageBox->setText(message.c_str());
        messageBox->setWindowTitle(tr("Settings test failed"));
        messageBox->setStandardButtons(QMessageBox::Ok);
        messageBox->exec();
        delete messageBox;
        qDebug() << e.what();
    }
}

void DriverDialog::modelBound()
{
    _model.apiType = IDriver::ApiType(ui->driverType->currentData().toInt());
    _model.bufferCount = unsigned(ui->bufferCount->value());
    _model.bufferSize = unsigned(ui->bufferSize->currentData().toInt());

    _model.inputChannels = 0;
    _model.inputDeviceId = 0;
    _model.outputChannels = 0;
    _model.outputDeviceId = 0;

    if (ui->inputDevice->currentData().toInt() >= 0) {
        _model.inputChannels = unsigned(ui->inChannels->value());
        _model.inputDeviceId = unsigned(ui->inputDevice->currentData().toInt());
    }

    if (ui->outputDevice->currentData().toInt() >= 0) {
        _model.outputChannels = unsigned(ui->outChannels->value());
        _model.outputDeviceId = unsigned(ui->outputDevice->currentData().toInt());
    }

    _model.isDuplex = ui->duplexMode->isChecked();
    _model.isRealtime = ui->realtime->isChecked();
    _model.isStoppingOnXrun = ui->xrunStop->isChecked();
    _model.isTested = false;

    _model.priority = ui->priority->value();
    _model.sampleFormat = IDriver::SampleFormat(
                ui->sampleFormat->currentData().toInt());
    _model.sampleRate = unsigned(ui->sampleRate->currentData().toInt());
}

void DriverDialog::modelInitialize(IDriver::ApiType apiType)
{
    const IDriver::Api &api = _driver->apiInfo(apiType);

    _model.apiType = apiType;
    _model.name = WEE_APPLICATION_NAME;
    _model.inputDeviceId = api.defaultInput;
    _model.inputChannels = DEFAULT_CHANNEL_COUNT;
    _model.outputDeviceId = api.defaultOutput;
    _model.outputChannels = DEFAULT_CHANNEL_COUNT;
    _model.bufferCount = DEFAULT_BUFFER_COUNT;
    _model.bufferSize = DEFAULT_BUFFER_SIZE;
    _model.isStoppingOnXrun = DEFAULT_IS_BREAK_ON_XRUN;
    _model.isDuplex = DEFAULT_IS_DUPLEX;
    _model.isRealtime = DEFAULT_IS_REALTIME;
    _model.priority = DEFAULT_REALTIME_PRIORITY;
    _model.isTested = false;
    _model.sampleFormat = DEFAULT_SAMPLE_FORMAT;
    _model.sampleRate = DEFAULT_SAMPLE_RATE;

    if (api.deviceCount) {
        _model.sampleFormat = selectFormat(
                    api.devices[api.defaultOutput].sampleFormats,
                    api.devices[api.defaultInput].sampleFormats,
                    _model.isDuplex);
        _model.sampleRate = selectSampleRate(
                    api.devices[api.defaultOutput].sampleRates,
                    api.devices[api.defaultOutput].sampleRateCount,
                    api.devices[api.defaultInput].sampleRates,
                    api.devices[api.defaultInput].sampleRateCount,
                    _model.isDuplex);
    }
}

void DriverDialog::modelLoad()
{
    const IDriver::DriverInfo &info = _driver->info();
    QSettings settings;
    settings.beginGroup("audio");

    QString apiName = settings.value("driverType", "").toString();
    IDriver::ApiType apiType = IDriver::ApiDefault;

    if (!apiName.isEmpty()) {
        for (unsigned i = 0; i < info.apiCount; i++) {
            if (apiName.compare(info.names[i], Qt::CaseInsensitive) == 0) {
                apiType = info.apis[i];
                break;
            }
        }
    }

    const IDriver::Api &api = _driver->apiInfo(apiType);
    const QString inputName = settings.value("inputDevice").toString();
    const QString outputName = settings.value("outputDevice").toString();
    const QString formatName =
            settings.value("sampleFormat").toString();

    modelInitialize(apiType);

    for (unsigned i = 0; i < api.deviceCount; i++) {
        if (api.devices[i].name == inputName) {
            _model.inputDeviceId = i;
        }

        if (api.devices[i].name == outputName) {
            _model.outputDeviceId = i;
        }
    }

    for (auto it = _formatMap.begin(); it != _formatMap.end(); ++it) {
        if (it->compare(formatName, Qt::CaseInsensitive) == 0) {
            _model.sampleFormat = it.key();
            break;
        }
    }

#define GET_FIELD(__name, __variant, __type) \
    _model.__name = __type(settings.value(#__name, _model.__name).to##__variant());

    GET_FIELD(bufferCount,      Int, unsigned);
    GET_FIELD(bufferSize,       Int, unsigned);
    GET_FIELD(inputChannels,    Int, unsigned);
    GET_FIELD(isStoppingOnXrun, Bool,);
    GET_FIELD(isDuplex,         Bool,);
    GET_FIELD(isRealtime,       Bool,);
    GET_FIELD(isTested,         Bool,);
    GET_FIELD(outputChannels,   Int, unsigned);
    GET_FIELD(priority,         Int,);
    GET_FIELD(sampleRate,       Int, unsigned);

#undef GET_FIELD

    settings.endGroup();
    controlsUpdate();
}

void DriverDialog::modelSave()
{
    const IDriver::DriverInfo &info = _driver->info();
    const IDriver::Api &api = _driver->apiInfo(_model.apiType);
    QSettings settings;

    settings.beginGroup("audio");

    for (unsigned i = 0; i < info.apiCount; i++) {
        if (info.apis[i] == _model.apiType) {
            settings.setValue("driverType", info.names[i]);
            break;
        }
    }

    settings.setValue("outputDevice", api.devices[_model.outputDeviceId].name);
    settings.setValue("inputDevice", api.devices[_model.inputDeviceId].name);
    settings.setValue("sampleFormat", _formatMap[_model.sampleFormat]);

#define SET_FIELD(__name) \
    settings.setValue(#__name, _model.__name)

    SET_FIELD(inputChannels);
    SET_FIELD(outputChannels);
    SET_FIELD(isStoppingOnXrun);
    SET_FIELD(isDuplex);
    SET_FIELD(isRealtime);
    SET_FIELD(isTested);
    SET_FIELD(priority);
    SET_FIELD(sampleRate);
    SET_FIELD(bufferCount);
    SET_FIELD(bufferSize);

#undef SET_FIELD

    settings.endGroup();
}

void DriverDialog::modelUpdate()
{
    if (!_isUpdateLocked) {
        memset(&_model, 0, sizeof _model);
        _model.name = WEE_APPLICATION_NAME;
        modelBound();
        updateStatus();
    }
}

void DriverDialog::populateBufferSizes()
{
    ui->bufferSize->clear();

    for (unsigned n = 8; n < 10000; n <<= 1) {
        ui->bufferSize->addItem(QString::number(n), n);
    }
}

void DriverDialog::populateDriverTypes()
{
    if (!ui->driverType->count()) {
        ui->driverType-> clear();
        auto &info = _driver->info();

        for (unsigned i = 0; i < info.apiCount; i++) {
            ui->driverType->addItem(info.names[i], info.apis[i]);
        }

        ui->driverType->setEnabled(info.apiCount > 1);
    }
}

void DriverDialog::populateInputs(IDriver::ApiType type)
{
    ui->inputDevice->clear();

    if (ui->duplexMode->isChecked()) {
        const IDriver::Api &api = _driver->apiInfo(type);

        for (unsigned i = 0; i < api.deviceCount; i++) {
            auto &device = api.devices[i];

            if (device.channelsDuplex > 0 || device.channelsInput > 0) {
                ui->inputDevice->addItem(device.name, i);
            }
        }
    }

    int count = ui->inputDevice->count();
    ui->inputDevice->setEnabled(count > 1);
    ui->inChannels->setEnabled(count > 0);

    if (!count) {
        bool isDuplex = ui->duplexMode->isChecked();
        bool hasOutput = ui->outputDevice->count() > 0
                && ui->outputDevice->itemData(0).toInt() >= 0;

        if (!isDuplex && hasOutput) {
            ui->inputDevice->addItem(tr("(not used)"), -1);
        } else {
            ui->inputDevice->addItem(tr("(unavailable)"), -1);
        }
    }

}

void DriverDialog::populateOutputs(IDriver::ApiType type)
{
    ui->outputDevice->clear();
    const IDriver::Api &api = _driver->apiInfo(type);

    for (unsigned i = 0; i < api.deviceCount; i++) {
        auto &device = api.devices[i];

        if (device.channelsDuplex > 0 || device.channelsOutput > 0) {
            ui->outputDevice->addItem(device.name, i);
        }
    }

    int count = ui->outputDevice->count();
    ui->outputDevice->setEnabled(count > 1);
    ui->outChannels->setEnabled(count > 0);

    if (!count) {
        ui->outputDevice->addItem(tr("(unavailable)"), -1);
    }
}

void DriverDialog::populateSampleRates(IDriver::ApiType type)
{
    ui->sampleRate->clear();
    const IDriver::Api &api = _driver->apiInfo(type);

    int inputDeviceId = ui->inputDevice->currentData().toInt();
    int outputDeviceId = ui->outputDevice->currentData().toInt();
    bool hasInput = inputDeviceId >= 0;
    bool hasOutput = outputDeviceId >= 0;

    if (hasOutput) {
        auto &outputDevice = api.devices[outputDeviceId];

        for (unsigned i = 0; i < outputDevice.sampleRateCount; i++) {
            unsigned rate = outputDevice.sampleRates[i];

            if (hasInput) {
                auto &inputDevice = api.devices[inputDeviceId];

                for (unsigned j = 0; j < inputDevice.sampleRateCount; j++) {
                    if (rate == inputDevice.sampleRates[j]) {
                        ui->sampleRate->addItem(QString::number(rate), rate);
                    }
                }
            } else {
                ui->sampleRate->addItem(QString::number(rate), rate);
            }
        }
    } else if (hasInput) {
        auto &inputDevice = api.devices[inputDeviceId];

        for (unsigned i = 0; i < inputDevice.sampleRateCount; i++) {
            unsigned rate = inputDevice.sampleRates[i];
            ui->sampleRate->addItem(QString::number(rate), rate);
        }
    }
}

void DriverDialog::populateSampleFormats(IDriver::ApiType type)
{
    ui->sampleFormat->clear();

    if (ui->driverType->currentIndex() < 0) {
        return;
    }

    unsigned flags = 0;
    const IDriver::Api &api = _driver->apiInfo(type);

    int inputDeviceId = ui->inputDevice->currentData().toInt();
    int outputDeviceId = ui->outputDevice->currentData().toInt();
    bool hasInput = inputDeviceId >= 0;
    bool hasOutput = outputDeviceId >= 0;

    if (hasOutput) {
        auto &outputDevice = api.devices[outputDeviceId];
        flags = outputDevice.sampleFormats;
    }

    if (hasInput) {
        auto &inputDevice = api.devices[inputDeviceId];

        if (hasOutput) {
            flags &= inputDevice.sampleFormats;
        } else {
            flags = inputDevice.sampleFormats;
        }
    }

    for (auto it = _formatMap.begin(); it != _formatMap.end(); ++it) {
        if (flags & it.key()) {
            ui->sampleFormat->addItem(it.value(), it.key());
        }
    }
}

void DriverDialog::selectByValue(QComboBox *target, unsigned value)
{
    bool isFound = false;
    int closest = 0;
    int closestIndex = 0;

    for (int i = 0; i < target->count(); i++) {
        int data = target->itemData(i).toInt();

        if (unsigned(data) == value) {
            target->setCurrentIndex(i);
            isFound = true;
            break;
        } else if (std::abs(data - int(value)) < std::abs(closest - int(value))) {
            closest = data;
            closestIndex = i;
        }
    }

    if (!isFound) {
        target->setCurrentIndex(closestIndex);
    }
}

IDriver::SampleFormat DriverDialog::selectFormat(
        unsigned outFlags, unsigned inFlags, bool isDuplex,
        IDriver::SampleFormat defaultFormat)
{
    IDriver::SampleFormat result = defaultFormat;

    unsigned flags = outFlags;

    if (isDuplex) {
        flags |= inFlags;
    }

    if (!(flags & result)) {
        auto keys = _formatMap.keys();
        auto it = keys.constEnd();

        while (it != keys.constBegin()) {
            --it;

            if (flags & *it) {
                result = *it;
                break;
            }
        }
    }

    return result;
}

unsigned DriverDialog::selectSampleRate(const unsigned *outRates, unsigned outRateCount,
                                        const unsigned *inRates, unsigned inRateCount,
                                        bool isDuplex, unsigned defaultRate)
{
    unsigned result = defaultRate;
    std::list<unsigned> available;

    for (unsigned i = 0; i < outRateCount; i++) {
        if (isDuplex) {
            for (unsigned j = 0; j < inRateCount; j++) {
                if (inRates[j] == outRates[i]) {
                    available.push_back(inRates[j]);
                    break;
                }
            }
        } else {
            available.push_back(outRates[i]);
        }
    }

    if (std::find(available.begin(), available.end(), result)
            == available.end())
    {
        result = available.front();
    }

    return result;
}

void DriverDialog::setStatus(const QString &themeIcon, const QString &message)
{
    _statusIcon->setPixmap(QIcon::fromTheme(themeIcon).pixmap(18));
    _statusText->setText(message);
}

void DriverDialog::updateStatus()
{
    if (_model.isTested) {
        setStatus("emblem-default", tr("Successfully tested"));
        _latencyText->setText(tr("Latency: <b>%1ms</b>").arg(_latency));
    } else {
        _latencyText->clear();

        if (!ui->outputDevice->count()
                || ui->outputDevice->itemData(0) < 0)
        {
            setStatus("dialog-error", tr("Not available"));
        } else {
            setStatus("", "");
        }
    }
}

void DriverDialog::on_DriverDialog_accepted()
{
    modelSave();
}

void DriverDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    auto btn = dynamic_cast<QPushButton *>(button);

    if (ui->buttonBox->button(QDialogButtonBox::Reset) == btn) {
        modelLoad();
    } else if (ui->buttonBox->button(QDialogButtonBox::Apply) == btn) {
        modelApply();
    }
}

void DriverDialog::on_driverType_currentIndexChanged(int index)
{
    if (!_isUpdateLocked) {
        IDriver::ApiType type = static_cast<IDriver::ApiType>(
                    ui->driverType->itemData(index).toInt());

        if (_model.apiType != type) {
            modelInitialize(type);
        }

        controlsUpdate();
    }
}
