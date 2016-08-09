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
    unsigned bufferFrames() const;
    void connect(std::shared_ptr<IDriver::ConnectOptions> options);
    const IDriver::DriverInfo &info() const;
    int latency() const;
    std::shared_ptr<IDriver::ConnectOptions> options() const;

    template<typename T>
    std::shared_ptr<Processor<T>> root() const;

    unsigned sampleRate() const;
    Sound::Type sampleType() const;

    template<typename T>
    void setRoot(std::shared_ptr<Processor<T>> processor);

    void start();
    void stop();
    double time() const;

private:
    template<typename T>
    class Runtime {
    public:
        Runtime();
        ~Runtime();
        std::shared_ptr<Buffer<T>> inputBuffer;
        std::shared_ptr<Buffer<T>> outputBuffer;
        std::shared_ptr<Processor<T>> rootProcessor;
    };

    unsigned _bufferFrames;
    std::shared_ptr<IDriver::ConnectOptions> _options;
    std::shared_ptr<IDriver> _provider;
    std::shared_ptr<void> _runtime;
    Sound::Type _sampleType;
};

#endif // DRIVER_H
