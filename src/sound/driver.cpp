#include "sample.h"
#include "sound.h"
#include "driver.h"
#include "shared/server.h"

Driver::Driver()
    : _provider(PLUGIN_FACTORY(IDriver))
{

}

Driver::~Driver()
{

}

void Driver::abort()
{
    _provider->control(IDriver::ControlAbort);
}

const IDriver::Api &Driver::apiInfo(IDriver::ApiType type) const
{
    return *_provider->apiInfo(type);
}

unsigned Driver::bufferFrames() const
{
    return _bufferFrames;
}

void Driver::connect(const IDriver::ConnectOptions &options)
{
    _provider = PLUGIN_FACTORY(IDriver);
    _bufferFrames = _provider->connect(options);

    std::map<unsigned, Sound::Type> formatsMap = {
        {IDriver::SampleFloat32, Sound::TypeFloat32},
        {IDriver::SampleFloat64, Sound::TypeFloat64},
        {IDriver::SampleInt8, Sound::TypeInt8},
        {IDriver::SampleInt16, Sound::TypeInt16},
        {IDriver::SampleInt32, Sound::TypeInt32},
    };

    _sampleType = formatsMap.at(options.sampleFormat);
    _options = std::make_shared<IDriver::ConnectOptions>(options);
}

void Driver::disconnect()
{
    _provider->disconnect();
}

const IDriver::DriverInfo &Driver::info() const
{
    return *_provider->driverInfo();
}

unsigned Driver::latency() const
{
    return _provider->streamInfo()->latency;
}

std::shared_ptr<IDriver::ConnectOptions> Driver::options() const
{
    return _options;
}

template<typename T>
Processor<T> *Driver::root() const
{
    if (sampleType() != Sound::Object<T>::type()) {
        std::string message = "Attempt to get root processor with wrong sample format";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    auto runtime = reinterpret_cast<Runtime<T> *>(_runtime.get());
    return runtime->root().get();
}

unsigned Driver::sampleRate() const
{
    return _provider->streamInfo()->sampleRate;
}

Sound::Type Driver::sampleType() const
{
    return _sampleType;
}

void Driver::start()
{
    _provider->control(IDriver::ControlStart);
}

void Driver::stop()
{
    _provider->control(IDriver::ControlStop);
}

double Driver::time() const
{
    return _provider->time();
}

template<typename T>
void Driver::setRoot(Processor<T> *processor)
{
    if (sampleType() != Sound::Object<T>::type()) {
        std::string message = "Wrong root processor sample format";
        LOG(ERROR, message);
        throw std::runtime_error(message);
    }

    Runtime<T> *pruntime = new Runtime<T>(
                std::shared_ptr<Processor<T>>(processor),
                _options->outputChannels,
                _bufferFrames,
                latency(),
                sampleRate());

    _runtime = std::shared_ptr<void>(pruntime, [](void *ptr) {
        Runtime<T> *pruntime = reinterpret_cast<Runtime<T> *>(ptr);
        delete pruntime;
    });
}

template<typename T>
Driver::Runtime<T>::Runtime(
        std::shared_ptr<Processor<T>> rootProcessor,
        unsigned channels, unsigned frames,
        unsigned latency, unsigned sampleRate)
    : _inputBuffer(channels, frames)
    , _outputBuffer(channels, frames)
    , _rootProcessor(rootProcessor)
{
    _rootProcessor->kickIn(&_outputBuffer, &_inputBuffer,
                           latency, sampleRate);
}

template<typename T>
Driver::Runtime<T>::~Runtime()
{

}

template<typename T>
std::shared_ptr<Processor<T>> Driver::Runtime<T>::root()
{
    return _rootProcessor;
}
