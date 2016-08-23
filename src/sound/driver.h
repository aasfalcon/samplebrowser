#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include <memory>

#include "processor/base.h"
#include "shared/idriver.h"

namespace Sound {

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

    void init(std::shared_ptr<Processor::Base> root);

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
