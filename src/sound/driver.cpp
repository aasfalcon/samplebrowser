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

unsigned Driver::bufferFrames() const
{
    return _bufferFrames;
}

void Driver::connect(std::shared_ptr<IDriver::ConnectOptions> options)
{
    _provider = PLUGIN_FACTORY(IDriver);
    _bufferFrames = _provider->connect(*options);

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
        if (formats[st] == options->sampleFormat) {
            _sampleType = Sound::Type(st);
            break;
        }
    }

    if (_sampleType == Sound::TypeCount) {
        throw std::runtime_error("Unknown sample format");
    }
}

const IDriver::DriverInfo &Driver::info() const
{
    return *_provider->driverInfo();
}

int Driver::latency() const
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
    return runtime->rootProcessor;
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

    Runtime<T> *runtime = new Runtime<T>();
    runtime->inputBuffer = std::shared_ptr<Buffer<T>>(
                new Buffer<T>(_options->inputChannels, _bufferFrames));
    runtime->outputBuffer = std::shared_ptr<Buffer<T>>(
                new Buffer<T>(_options->outputChannels, _bufferFrames));
    runtime->rootProcessor = processor;

    _runtime = std::shared_ptr<void>(runtime, [](void *ptr) {
        Runtime<T> *pobject = reinterpret_cast<Runtime<T> *>(ptr);
        delete pobject;
    });

    processor->kickIn(runtime->outputBuffer, runtime->inputBuffer,
                  latency(), sampleRate());
}

template<typename T> Driver::Runtime<T>::Runtime() {}
template<typename T> Driver::Runtime<T>::~Runtime() {}

#define ROOT_PROPERTY(__type) \
    template<> std::shared_ptr<Processor<Sound::__type>> \
    Driver::root() const; \
    template<> void \
    Driver::setRoot(std::shared_ptr<Processor<Sound::__type>> value);

SOUND_SPECIALIZE(ROOT_PROPERTY)
SOUND_INSTANTIATE(Driver::Runtime)
