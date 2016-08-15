#ifndef DRIVER_H
#define DRIVER_H

#include <memory>
#include "processor.h"
#include "shared/idriver.h"

class Driver
{
public:
    Driver();
    virtual ~Driver();

    void abort();
    const IDriver::Api &apiInfo(IDriver::ApiType type) const;
    unsigned bufferFrames() const;
    void connect(const IDriver::ConnectOptions &options);
    void disconnect();
    const IDriver::DriverInfo &info() const;
    unsigned latency() const;
    std::shared_ptr<IDriver::ConnectOptions> options() const;

    template<typename T>
    Processor<T> *root() const;

    unsigned sampleRate() const;
    Sound::Type sampleType() const;

    template<typename T>
    void setRoot(Processor<T> *processor);

    void start();
    void stop();
    double time() const;

private:
    template<typename T>
    class Runtime {
    public:
        Runtime(std::shared_ptr<Processor<T>> root,
                unsigned channels, unsigned frames,
                unsigned latency, unsigned sampleRate);
        ~Runtime();

        std::shared_ptr<Processor<T>> root();

    private:
        Buffer<T> _inputBuffer;
        Buffer<T> _outputBuffer;
        std::shared_ptr<Processor<T>> _rootProcessor;
    };

    unsigned _bufferFrames;
    std::shared_ptr<IDriver::ConnectOptions> _options;
    std::shared_ptr<IDriver> _provider;
    std::shared_ptr<void> _runtime;
    Sound::Type _sampleType;
};

SOUND_INSTANTIATE_DECLARATION(Driver::Runtime)

#endif // DRIVER_H
