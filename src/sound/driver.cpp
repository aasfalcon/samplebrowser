#include "driver.h"
#include "runtime.h"
#include "shared/log.h"
#include "shared/server.h"

using namespace Sound;

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

template <typename T>
void Driver::init(std::shared_ptr<Processor<T> > root)
{
    if (sampleType() != TypeInt24E && sampleType() != root->type()) {
        RUNTIME_ERROR("Wrong root processor sample format");
    }

    Runtime<T>* pruntime = new Runtime<T>(
        root, _options->outputChannels, _bufferFrames,
        latency(), sampleRate(), sampleType());

    _runtime = std::shared_ptr<void>(pruntime, [](void* ptr) {
        delete reinterpret_cast<Runtime<T>*>(ptr);
    });

    _provider->setProcess(&Runtime<T>::process, pruntime);
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

#define DRIVER_INIT(__type) \
    template void Driver::init<__type>(std::shared_ptr<Processor<__type> > root);

SOUND_INSTANTIATE_METHOD(DRIVER_INIT);
