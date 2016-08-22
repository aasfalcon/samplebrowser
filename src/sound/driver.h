#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include "shared/idriver.h"
#include <memory>

namespace Sound {

template <typename T>
class Processor;

class Driver {
public:
    Driver();
    virtual ~Driver();

    void abort();
    const IDriver::Api& apiInfo(IDriver::ApiType type) const;
    unsigned bufferFrames() const;
    void connect(const IDriver::ConnectOptions& options);
    void disconnect();
    const IDriver::DriverInfo& info() const;
    unsigned latency() const;
    std::shared_ptr<IDriver::ConnectOptions> options() const;

    unsigned sampleRate() const;
    Type sampleType() const;

    template <typename T>
    void init(std::shared_ptr<Processor<T> > root);

    void start();
    void stop();
    double time() const;

private:
    unsigned _bufferFrames;
    std::shared_ptr<IDriver::ConnectOptions> _options;
    std::shared_ptr<IDriver> _provider;
    std::shared_ptr<void> _runtime;
    Type _sampleType;
};
}

#endif // SOUND_DRIVER_H
