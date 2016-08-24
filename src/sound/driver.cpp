#include "driver.h"
#include "processor/root.h"
#include "shared/log.h"
#include "shared/server.h"

using namespace Sound;

Driver::Driver()
    : _provider(PLUGIN_FACTORY(IDriver))
    , _root(nullptr)
{
}

Driver::~Driver()
{
}

void Driver::abort()
{
    _provider->control(IDriver::ControlAbort);
}

const IDriver::Api& Driver::apiInfo(IDriver::ApiType type) const
{
    return *_provider->apiInfo(type);
}

unsigned Driver::bufferFrames() const
{
    return _bufferFrames;
}

void Driver::connect(const IDriver::ConnectOptions& options)
{
    _provider = PLUGIN_FACTORY(IDriver);
    _bufferFrames = _provider->connect(options);

    std::map<unsigned, Type> formatsMap = {
        { IDriver::SampleFloat32, TypeFloat32 },
        { IDriver::SampleFloat64, TypeFloat64 },
        { IDriver::SampleInt8, TypeInt8 },
        { IDriver::SampleInt16, TypeInt16 },
        { IDriver::SampleInt24, TypeInt24E },
        { IDriver::SampleInt32, TypeInt32 },
    };

    _sampleType = formatsMap.at(options.sampleFormat);
    _options = std::make_shared<IDriver::ConnectOptions>(options);
}

void Driver::disconnect()
{
    _provider->disconnect();
}

const IDriver::DriverInfo& Driver::info() const
{
    return *_provider->driverInfo();
}

void Driver::init()
{
    _runtime.bus = &_messageBus;
    _runtime.channelsInput = _options->inputChannels;
    _runtime.channelsOutput = _options->outputChannels;
    _runtime.frames = _bufferFrames;
    _runtime.latency = latency();
    _runtime.sampleRate = sampleRate();
    _runtime.status = 0;
    _runtime.rawInput = nullptr;
    _runtime.rawOutput = nullptr;

    _runtime.rawSampleFormat = _runtime.sampleFormat = sampleType();

    if (TypeInt24E == _runtime.sampleFormat) {
        _runtime.sampleFormat = TypeFloat32;
    }

    _root = std::shared_ptr<Processor::Base>(
        Processor::Factory::Root::create(_runtime.sampleFormat));
    _root->set(Parameter::Processor::Runtime, &_runtime);

    _provider->setProcess(&Driver::process, this);
    LOG(TRACE, "Driver initialized");
}

unsigned Driver::latency() const
{
    return _provider->streamInfo()->latency;
}

std::shared_ptr<IDriver::ConnectOptions> Driver::options() const
{
    return _options;
}

IDriver::Control Driver::process(const IDriver::ProcessParams& data)
{
    Driver* driver = reinterpret_cast<Driver*>(data.object);
    driver->_runtime.rawInput = data.input;
    driver->_runtime.rawOutput = data.output;
    driver->_runtime.status = data.status;
    driver->_runtime.time = data.time;
    driver->_root->processEntryPoint();
    return IDriver::ControlContinue;
}

Processor::Base* Driver::root() const
{
    if (!_root) {
        LOGIC_ERROR("Trying to get root processor while driver is not initialized");
    }

    return _root.get();
}

unsigned Driver::sampleRate() const
{
    return _provider->streamInfo()->sampleRate;
}

Type Driver::sampleType() const
{
    return _sampleType;
}

void Driver::start()
{
    auto si = _provider->streamInfo();

    if (!si->isRunning) {
        if (!si->isOpen) {
            LOGIC_ERROR("Attempt to start driver while not open");
        }

        this->init();
        _root->perform(Command::Processor::Init);
        _provider->control(IDriver::ControlStart);
    }
}

void Driver::stop()
{
    _provider->control(IDriver::ControlStop);
}

double Driver::time() const
{
    return _provider->time();
}
