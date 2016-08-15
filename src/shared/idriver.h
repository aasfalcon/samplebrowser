#ifndef IDRIVER_H
#define IDRIVER_H

#include "interface.h"
#include "../sound/sound.h"

struct IDriver: Interface {
    enum SampleFormat {
        SampleInt8      = 0x01,
        SampleInt16     = 0x02,
        SampleInt24     = 0x04,
        SampleInt32     = 0x08,
        SampleFloat32   = 0x10,
        SampleFloat64   = 0x20,
    };

    enum ApiType {
        ApiDefault,
        ApiLinuxAlsa,
        ApiLinuxPulse,
        ApiLinuxOss,
        ApiLinuxJack,
        ApiMacosxCore,
        ApiWindowsWasapi,
        ApiWindowsAsio,
        ApiWindowsDs,
        ApiDummy,

        ApiCount
    };

    enum Status {
        StatusOk                = 0x00,
        StatusInputOverflow     = 0x01,
        StatusOutputUnderflow   = 0x02,
    };

    enum Control {
        ControlContinue = 0,
        ControlStart = ControlContinue,
        ControlAbort,
        ControlStop,
    };

    struct Device {
        char *name;
        unsigned channelsDuplex;
        unsigned channelsInput;
        unsigned channelsOutput;
        unsigned sampleFormats; // SampleFormat flags
        unsigned sampleRateCount;
        unsigned *sampleRates;
    };

    struct Api {
        unsigned deviceCount;
        Device *devices;
        unsigned defaultInput;
        unsigned defaultOutput;
        const char *name;
        ApiType type;
    };

    struct DriverInfo {
        unsigned apiCount;
        ApiType *apis;
        const char **names;
        ApiType defaultApi;
    };

    struct Stream {
        unsigned deviceId;
        unsigned channelFirst;
        unsigned channels;
        unsigned sampleRate;
    };

    struct ProcessParams {
        unsigned channelsInput;
        unsigned channelsOutput;
        unsigned frames;
        void *input;
        unsigned latency;
        void *output;
        unsigned sampleRate;
        unsigned status; // StausXXX flags
        double time;
    };

    struct StreamInfo {
        unsigned frames;
        bool isOpen;
        bool isRunning;
        double time;
        unsigned sampleRate;
        unsigned latency;
        Stream input;
        Stream output;
    };

    struct ConnectOptions {
        unsigned bufferCount;
        unsigned bufferSize;
        unsigned inputChannels;
        unsigned inputDeviceId;
        bool isDuplex;
        bool isRealtime;
        bool isTested;
        ApiType apiType;
        const char *name;
        unsigned outputChannels;
        unsigned outputDeviceId;
        int priority;
        SampleFormat sampleFormat;
        unsigned sampleRate;
        bool isStoppingOnXrun;
    };

    typedef Control (*Process)(const ProcessParams &data);

    virtual const Api *apiInfo(ApiType type) = 0;
    virtual unsigned connect(const ConnectOptions &options) = 0;
    virtual void control(Control task) = 0;
    virtual void disconnect() = 0;
    virtual const DriverInfo *driverInfo() = 0;
    virtual void setProcess(Process callback) = 0;
    virtual const StreamInfo *streamInfo() = 0;
    virtual double time() const = 0;
};

#endif // IDRIVER_H
