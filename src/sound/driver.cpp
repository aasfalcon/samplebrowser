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

    unsigned formats[Sound::TypeCount] = {
        IDriver::SampleFloat32,
        IDriver::SampleFloat64,
        IDriver::SampleInt8,
        IDriver::SampleInt16,
        IDriver::SampleInt24,
        IDriver::SampleInt32,
    };

    _sampleType = Sound::TypeCount;

    for (unsigned st = 0; st < Sound::TypeCount; st++) {
        if (formats[st] == options.sampleFormat) {
            _sampleType = Sound::Type(st);
            break;
        }
    }

    if (_sampleType == Sound::TypeCount) {
        throw std::runtime_error("Unknown sample format");
    }

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
std::shared_ptr<Processor<T>> Driver::root() const
{
    if (sampleType() != Object<T>::type()) {
        throw std::runtime_error("Attempt to get root processor with wrong sample format");
    }

    auto runtime = reinterpret_cast<Runtime<T> *>(_runtime.get());
    return runtime->rootProcessor();
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
void Driver::setRoot(std::shared_ptr<Processor<T>> processor)
{
    if (sampleType() != Object<T>::type()) {
        throw std::runtime_error("Wrong root processor sample format");
    }

    Runtime<T> *pruntime = new Runtime<T>(
                processor,
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
        std::shared_ptr<Processor<T> > &rootProcessor,
        unsigned channels, unsigned frames,
        unsigned latency, unsigned sampleRate)
    : _inputBuffer(new Buffer<T>(channels, frames))
    , _outputBuffer(new Buffer<T>(channels, frames))
    , _rootProcessor(rootProcessor)
{
    _rootProcessor->kickIn(_outputBuffer, _inputBuffer,
                           latency, sampleRate);
}

template<typename T>
Driver::Runtime<T>::~Runtime()
{

}

template<typename T>
std::shared_ptr<Processor<T>> Driver::Runtime<T>::rootProcessor()
{
    return _rootProcessor;
}

#define ROOT_PROPERTY(__type) \
    template std::shared_ptr<Processor<Sound::__type>> \
    Driver::root() const; \
    template void \
    Driver::setRoot(std::shared_ptr<Processor<Sound::__type>> value);

SOUND_SPECIALIZE(ROOT_PROPERTY)
SOUND_INSTANTIATE(Driver::Runtime)
