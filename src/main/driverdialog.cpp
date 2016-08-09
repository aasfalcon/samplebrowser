#include <algorithm>
#include <list>
#include <RtAudio.h>
#include <QPushButton>

#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>

#include "driverdialog.h"
#include "ui_driverdialog.h"

#include "sound/driver.h"

DriverDialog::DriverDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DriverDialog)
    , _apiCurrent(IDriver::ApiDefault)
    , _apiIndex(0)
    , _driver(new Driver)
    , _model(NULL)
    , _status(new QStatusBar(this))
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Retry)->setText(tr("Check"));
    ui->statusLayout->addWidget(_status);

    QLabel *label = new QLabel();
    label->setPixmap(QIcon::fromTheme(
                         QStringLiteral("list-add"),
                         QIcon(QStringLiteral(":/images/add"))).pixmap(18));
    _status->addWidget(label);
    _status->addWidget(new QLabel(tr("Polling devices...")));

    QFuture<void> future = QtConcurrent::run([this]() {
        settingsLoad();
        ui->chooseDriverGroup->setEnabled(true);
        ui->detailsGroup->setEnabled(true);
        ui->buttonBox->setEnabled(true);
    });

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    watcher->setFuture(future);
    connect(watcher, SIGNAL(finished()), this, SLOT(on_watcher_finished()));
}

DriverDialog::~DriverDialog()
{
    delete ui;
}

void DriverDialog::populateBufferSizes()
{
    ui->bufferSize->clear();

    for (unsigned n = 8; n < 10000; n <<= 1) {
        ui->bufferSize->addItem(QString::number(n), n);
    }

    selectByValue(ui->bufferSize, _model->bufferSize);
}

void DriverDialog::populateDriverTypes()
{
    ui->driverType-> clear();
    auto &info = _driver->info();

    for (unsigned i = 0; i < info.apiCount; i++) {
        auto &api = info.apis[i];

        if (_models[api.type].isActive) {
            ui->driverType->addItem(api.name, api.type);
        }
    }

    setApiType(_apiCurrent);
    selectByValue(ui->driverType, _apiCurrent);
    ui->duplexMode->setChecked(_model->isDuplex);
}

void DriverDialog::populateInputs()
{
    ui->inputDevice->clear();

    if (!ui->duplexMode->isChecked()) {
        return;
    }

    auto &info = _driver->info();
    auto &api = info.apis[_apiIndex];

    for (unsigned i = 0; i < api.deviceCount; i++) {
        auto &device = api.devices[i];

        if (device.channelsDuplex > 0 || device.channelsInput > 0) {
            ui->inputDevice->addItem(device.name, i);
        }
    }

    selectByValue(ui->inputDevice, _model->inputDeviceId);
}

void DriverDialog::populateOutputs()
{
    ui->outputDevice->clear();
    auto &info = _driver->info();
    auto &api = info.apis[_apiIndex];

    for (unsigned i = 0; i < api.deviceCount; i++) {
        auto &device = api.devices[i];

        if (device.channelsDuplex > 0 || device.channelsOutput > 0) {
            ui->outputDevice->addItem(device.name, i);
        }
    }

    selectByValue(ui->outputDevice, _model->outputDeviceId);
}

void DriverDialog::populateSampleRates()
{
    ui->sampleRate->clear();
    auto &info = _driver->info();
    auto &api = info.apis[_apiIndex];

    bool hasInput = ui->inputDevice->currentIndex() >= 0;
    bool hasOutput = ui->outputDevice->currentIndex() >= 0;

    if (hasOutput) {
        auto &outputDevice = api.devices[_model->outputDeviceId];

        for (unsigned i = 0; i < outputDevice.sampleRateCount; i++) {
            unsigned rate = outputDevice.sampleRates[i];

            if (hasInput) {
                auto &inputDevice = api.devices[_model->inputDeviceId];

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
        auto &inputDevice = api.devices[_model->inputDeviceId];

        for (unsigned i = 0; i < inputDevice.sampleRateCount; i++) {
            unsigned rate = inputDevice.sampleRates[i];
            ui->sampleRate->addItem(QString::number(rate), rate);
        }
    }

    selectByValue(ui->sampleRate, _model->sampleRate);
}

void DriverDialog::populateSampleFormat()
{
    ui->sampleFormat->clear();

    if (ui->driverType->currentIndex() < 0) {
        return;
    }

    unsigned flags = 0;
    auto &info = _driver->info();
    auto &api = info.apis[_apiIndex];

    bool hasInput = ui->inputDevice->currentIndex() >= 0;
    bool hasOutput = ui->outputDevice->currentIndex() >= 0;

    if (hasOutput) {
        auto &outputDevice = api.devices[_model->outputDeviceId];
        flags = outputDevice.sampleFormats;
    }

    if (hasInput) {
        auto &inputDevice = api.devices[_model->inputDeviceId];

        if (hasOutput) {
            flags &= inputDevice.sampleFormats;
        } else {
            flags = inputDevice.sampleFormats;
        }
    }

    if (RTAUDIO_SINT8 & flags) {
        ui->sampleFormat->addItem(tr("8 bit"), Sound::TypeInt8);
    }
    if (RTAUDIO_SINT16 & flags) {
        ui->sampleFormat->addItem(tr("16 bit"), Sound::TypeInt16);
    }
    if (RTAUDIO_SINT24 & flags) {
        ui->sampleFormat->addItem(tr("24 bit"), Sound::TypeInt24);
    }
    if (RTAUDIO_SINT32 & flags) {
        ui->sampleFormat->addItem(tr("32 bit"), Sound::TypeInt32);
    }
    if (RTAUDIO_FLOAT32 & flags) {
        ui->sampleFormat->addItem(tr("Float 32 bit"), Sound::TypeFloat32);
    }
    if (RTAUDIO_FLOAT64 & flags) {
        ui->sampleFormat->addItem(tr("Float 64 bit"), Sound::TypeFloat64);
    }

    selectByValue(ui->sampleFormat, _model->sampleFormat);
}

void DriverDialog::setApiType(IDriver::ApiType value) {
    auto &info = _driver->info();

    for (unsigned i = 0; i < info.apiCount; i++) {
        if (info.apis[i].type == value) {
            _apiIndex = i;
            _apiCurrent = value;
            break;
        }
    }

    _model = &_models[_apiCurrent];
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

void DriverDialog::settingsApply()
{
}

void DriverDialog::settingsLoad()
{
    memset(&_models, 0, sizeof _models);

    QSettings settings;
    auto &info = _driver->info();

    settings.beginGroup("audio");

    for (unsigned i = 0; i < info.apiCount; i++) {
        auto &api = info.apis[i];
        auto &model = _models[api.type];

        settings.beginGroup(api.name);

#define GET_FIELD(__name, __default, __variant, __type) \
    model.__name = __type(settings.value(#__name, __default).to##__variant());

        GET_FIELD(inputDeviceId,    api.defaultInput,           Int, unsigned);
        GET_FIELD(inputChannels,    DEFAULT_CHANNEL_COUNT,      Int, unsigned);
        GET_FIELD(outputDeviceId,   api.defaultOutput,          Int, unsigned);
        GET_FIELD(outputChannels,   DEFAULT_CHANNEL_COUNT,      Int, unsigned);
        GET_FIELD(bufferCount,      DEFAULT_BUFFER_COUNT,       Int, unsigned);
        GET_FIELD(bufferSize,       DEFAULT_BUFFER_SIZE,        Int, unsigned);

        GET_FIELD(isActive,         true,                       Bool,);
        GET_FIELD(isBreakOnXrun,    DEFAULT_IS_BREAK_ON_XRUN,   Bool,);
        GET_FIELD(isDuplex,         DEFAULT_IS_DUPLEX,          Bool,);

        Sound::Type defaultFormat = selectFormat(
                    api.devices[api.defaultOutput].sampleFormats,
                    api.devices[api.defaultInput].sampleFormats,
                    model.isDuplex);

        unsigned defaultRate = selectSampleRate(
                    api.devices[api.defaultOutput].sampleRates,
                    api.devices[api.defaultOutput].sampleRateCount,
                    api.devices[api.defaultInput].sampleRates,
                    api.devices[api.defaultInput].sampleRateCount,
                    model.isDuplex);

        GET_FIELD(sampleFormat, defaultFormat, Int, Sound::Type);
        GET_FIELD(sampleRate, defaultRate, Int, unsigned);

#undef GET_FIELD

        settings.endGroup();
    }

    setApiType(IDriver::ApiType(settings.value("selectedApi",
                                               info.defaultApi).toInt()));
    settings.endGroup();

    _isUpdateLocked = true;
    populateDriverTypes();
    populateOutputs();
    populateInputs();
    populateSampleFormat();
    populateSampleRates();
    populateBufferSizes();
    updateDisabled();
    _isUpdateLocked = false;
}

void DriverDialog::settingsSave()
{
    QSettings settings;
    auto &info = _driver->info();

    settings.beginGroup("audio");
    settings.setValue("selectedApi", _apiCurrent);

    for (unsigned i = 0; i < info.apiCount; i++) {
        auto &api = info.apis[i];
        auto &model = _models[api.type];

        settings.beginGroup(api.name);

#define SET_FIELD(__name) \
    settings.setValue(#__name, model.__name)

        SET_FIELD(inputDeviceId);
        SET_FIELD(inputChannels);
        SET_FIELD(outputDeviceId);
        SET_FIELD(outputChannels);
        SET_FIELD(isActive);
        SET_FIELD(isBreakOnXrun);
        SET_FIELD(isDuplex);
        SET_FIELD(sampleFormat);
        SET_FIELD(sampleRate);
        SET_FIELD(bufferCount);
        SET_FIELD(bufferSize);

#undef SET_FIELD

        settings.endGroup();
    }

    settings.endGroup();
}

Sound::Type DriverDialog::selectFormat(unsigned outFlags, unsigned inFlags,
                                       bool isDuplex, Sound::Type defaultFormat)
{
    Sound::Type result = defaultFormat;

    unsigned flags = outFlags;

    if (isDuplex) {
        flags |= inFlags;
    }

    if (flags) {
        std::list<Sound::Type> available;

        if (RTAUDIO_SINT8 & flags) {
            available.push_back(Sound::TypeInt8);
        }

        if (RTAUDIO_SINT16 & flags) {
            available.push_back(Sound::TypeInt16);
        }

        if (RTAUDIO_SINT24 & flags) {
            available.push_back(Sound::TypeInt24);
        }

        if (RTAUDIO_SINT32 & flags) {
            available.push_back(Sound::TypeInt32);
        }

        if (RTAUDIO_FLOAT32 & flags) {
            available.push_back(Sound::TypeFloat32);
        }

        if (RTAUDIO_FLOAT64 & flags) {
            available.push_back(Sound::TypeFloat64);
        }

        if (std::find(available.begin(), available.end(), result)
                == available.end())
        {
            result = available.back();
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

void DriverDialog::updateDisabled()
{
    _isUpdateLocked = true;
    bool hasInput = ui->inputDevice->count()
            && ui->inputDevice->itemData(0) >= 0;
    bool hasOutput = ui->outputDevice->count()
            && ui->outputDevice->itemData(0) >= 0;

    ui->driverType->setEnabled(ui->driverType->count() > 1);
    ui->inputDevice->setEnabled(ui->inputDevice->count() > 1);
    ui->outputDevice->setEnabled(ui->outputDevice->count() > 1);
    ui->sampleFormat->setEnabled(ui->sampleFormat->count() > 1);
    ui->sampleRate->setEnabled(ui->sampleRate->count() > 1);

    ui->bufferSize->setEnabled(hasInput || hasOutput);
    ui->bufferCount->setEnabled(hasInput || hasOutput);

    ui->inChannels->setEnabled(hasInput);
    ui->outChannels->setEnabled(hasOutput);

    if (!hasOutput) {
        ui->outputDevice->clear();
        ui->outputDevice->addItem(tr("(unavailable)"), -1);
    }

    if (!hasInput) {
        ui->inputDevice->clear();

        if (!ui->duplexMode->isChecked()) {
            ui->inputDevice->addItem(tr("(not used)"), -1);
        } else {
            ui->inputDevice->addItem(tr("(unavailable)"), -1);
        }
    }

    selectByValue(ui->bufferSize, _model->bufferSize);
    ui->bufferCount->setValue(int(_model->bufferCount));
    ui->inChannels->setValue(int(_model->inputChannels));
    ui->outChannels->setValue(int(_model->outputChannels));

    _isUpdateLocked = false;
}

void DriverDialog::on_watcher_finished()
{
    _status->showMessage(tr("Status: not checked"));
}

void DriverDialog::on_DriverDialog_accepted()
{
    settingsSave();
}

void DriverDialog::on_bufferCount_valueChanged(int value)
{
    if (!_isUpdateLocked) {
        _model->bufferCount = unsigned(value);
    }
}

void DriverDialog::on_bufferSize_currentIndexChanged(int /*index*/)
{
    if (!_isUpdateLocked) {
        _model->bufferSize = unsigned(ui->bufferSize->currentData().toInt());
    }
}

void DriverDialog::on_driverType_currentIndexChanged(int index)
{
    if (!_isUpdateLocked && index >= 0) {
        _isUpdateLocked = true;
        setApiType(IDriver::ApiType(ui->driverType->itemData(index).toInt()));
        populateInputs();
        populateOutputs();
        populateSampleFormat();
        populateSampleRates();
        updateDisabled();
        _isUpdateLocked = false;
    }
}

void DriverDialog::on_duplexMode_toggled(bool checked)
{
    if (!_isUpdateLocked) {
        _model->isDuplex = checked;

        _isUpdateLocked = true;
        populateInputs();
        updateDisabled();
        _isUpdateLocked = false;
    }
}

void DriverDialog::on_inChannels_valueChanged(int value)
{
    if (!_isUpdateLocked) {
        _model->inputChannels = unsigned(value);
    }
}

void DriverDialog::on_inputDevice_currentIndexChanged(int index)
{
    if (!_isUpdateLocked && index >= 0) {
        _model->inputDeviceId = unsigned(ui->inputDevice->currentData().toInt());

        _isUpdateLocked = true;
        populateSampleFormat();
        populateSampleRates();
        updateDisabled();
        _isUpdateLocked = false;
    }
}

void DriverDialog::on_outChannels_valueChanged(int value)
{
    if (!_isUpdateLocked) {
        _model->outputChannels = unsigned(value);
    }
}

void DriverDialog::on_outputDevice_currentIndexChanged(int index)
{
    if (!_isUpdateLocked && index >= 0) {
        _model->outputDeviceId = unsigned(ui->outputDevice->currentData().toInt());

        _isUpdateLocked = true;
        populateSampleFormat();
        populateSampleRates();
        updateDisabled();
        _isUpdateLocked = false;
    }
}

void DriverDialog::on_sampleFormat_currentIndexChanged(int /*index*/)
{
    if (!_isUpdateLocked) {
        _model->sampleFormat = Sound::Type(ui->sampleFormat->currentData().toInt());
    }
}

void DriverDialog::on_sampleRate_currentIndexChanged(int /*index*/)
{
    if (!_isUpdateLocked) {
        _model->sampleRate = unsigned(ui->sampleRate->currentData().toInt());
    }
}

void DriverDialog::on_xrunStop_toggled(bool checked)
{
    if (!_isUpdateLocked) {
        _model->isBreakOnXrun = checked;
    }
}

void DriverDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->button(QDialogButtonBox::Reset) ==
            dynamic_cast<QPushButton *>(button))
    {
        settingsLoad();
    }
}
