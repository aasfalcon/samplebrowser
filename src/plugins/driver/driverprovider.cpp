#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>

#include <RtAudio.h>

#include "driverprovider.h"

std::string DriverProvider::_names[] = {
    "<auto>",
    "ALSA",
    "PulseAudio",
    "OSS",
    "JACK",
    "MacOS X Core",
    "WASAPI",
    "ASIO",
    "DirectSound",
    "Dummy I/O"
};

DriverProvider::DriverProvider()
    : _process(NULL)
{
}

DriverProvider::~DriverProvider()
{
    disconnect();
}

const IDriver::Api *DriverProvider::apiInfo(IDriver::ApiType type)
{
    std::shared_ptr<Api> api = _apiInfos[type];

    if (!api) {
        auto deleteApi = [](Api *api) {
            for (unsigned i = 0; i < api->deviceCount; i++) {
                Device &device = api->devices[i];
                delete[] device.name;

                if (device.sampleRates) {
                    delete[] device.sampleRates;
                }
            }

            if (api->devices) {
                delete[] api->devices;
            }

            delete api;
        };

        api = std::shared_ptr<Api>(new Api(), deleteApi);

        api->type = type;

        RtAudio rt(RtAudio::Api(api->type));
        api->defaultInput = rt.getDefaultInputDevice();
        api->defaultOutput = rt.getDefaultOutputDevice();
        api->deviceCount = rt.getDeviceCount();

        if (api->deviceCount) {
            api->devices = new Device[api->deviceCount];
        }

        api->name = _names[api->type].c_str();

        for (unsigned i = 0; i < api->deviceCount; i++) {
            Device &device = api->devices[i];
            RtAudio::DeviceInfo info = rt.getDeviceInfo(i);

            device.channelsOutput = info.outputChannels;
            device.channelsInput = info.inputChannels;
            device.channelsDuplex = info.duplexChannels;

            device.name = new char[info.name.length() + 1];
            strcpy(device.name, info.name.c_str());

            device.sampleFormats = info.nativeFormats;
            device.sampleRateCount = info.sampleRates.size();
            device.sampleRates = new unsigned[device.sampleRateCount];
            memcpy(device.sampleRates, info.sampleRates.data(),
                   int(info.sampleRates.size() * sizeof(unsigned)));
        }

        _apiInfos[type] = api;
    }

    return _apiInfos.at(type).get();
}

unsigned DriverProvider::connect(const IDriver::ConnectOptions &options)
{
    if (_rtaudio) {
        throw std::runtime_error("Already connected");
    }

    _rtaudio = std::shared_ptr<RtAudio>(
                new RtAudio(RtAudio::Api(options.apiType)));

    std::unique_ptr<RtAudio::StreamParameters> iparams, oparams;

    if (options.outputChannels) {
        oparams.reset(new RtAudio::StreamParameters());
        oparams->deviceId = options.outputDeviceId;
        oparams->nChannels = options.outputChannels;
    }

    if (options.inputChannels && options.isDuplex) {
        iparams.reset(new RtAudio::StreamParameters());
        iparams->deviceId = options.inputDeviceId;
        iparams->nChannels = options.inputChannels;
    }

    RtAudio::StreamOptions so;
    so.flags = RTAUDIO_MINIMIZE_LATENCY | RTAUDIO_SCHEDULE_REALTIME;
    so.numberOfBuffers = options.bufferCount;
    so.priority = options.priority;
    so.streamName = options.name;

    unsigned bufferSize;
    _rtaudio->openStream(oparams.get(), iparams.get(),
                         options.sampleFormat, options.sampleRate,
                         &bufferSize, &DriverProvider::process,
                         this, &so, &DriverProvider::error);
    return bufferSize;
}

void DriverProvider::control(IDriver::Control command)
{
    if (!_rtaudio || !_rtaudio->isStreamOpen()) {
        throw std::runtime_error("Attempt to control stream which is not opened");
    }

    switch (command) {
    case ControlStart:
        _rtaudio->startStream();
        break;

    case ControlStop:
        _rtaudio->stopStream();
        break;

    case ControlAbort:
        _rtaudio->abortStream();
        break;
    }
}

void DriverProvider::disconnect()
{
    if (_rtaudio) {
        if (_rtaudio->isStreamRunning()) {
            _rtaudio->stopStream();
        }

        if (_rtaudio->isStreamOpen()) {
            _rtaudio->closeStream();
        }
    }

    _rtaudio.reset();
    _streamInfo.reset();
}

const IDriver::DriverInfo *DriverProvider::driverInfo()
{
    if (_driverInfo) {
        return _driverInfo.get();
    }

    std::vector<RtAudio::Api> apis;
    RtAudio::getCompiledApi(apis);

    auto deleteDI = [](DriverInfo *di) {
        delete[] di->apis;
        delete[] di->names;
        delete di;
    };

    _driverInfo = std::shared_ptr<DriverInfo>(new DriverInfo(), deleteDI);
    _driverInfo->apiCount = apis.size();
    _driverInfo->apis = new ApiType[_driverInfo->apiCount];
    _driverInfo->names = new const char *[_driverInfo->apiCount];

    for (unsigned i = 0; i < _driverInfo->apiCount; i++) {
        _driverInfo->apis[i] = ApiType(apis.at(i));
        _driverInfo->names[i] = _names[apis[i]].c_str();
    }
    memcpy(_driverInfo->apis, apis.data(), int(apis.size() * sizeof(ApiType)));

    RtAudio rt(RtAudio::UNSPECIFIED);
    _driverInfo->defaultApi = ApiType(rt.getCurrentApi());
    return _driverInfo.get();
}

void DriverProvider::setProcess(IDriver::Process callback)
{
    _process = callback;
}

const IDriver::StreamInfo *DriverProvider::streamInfo()
{
    if (!_rtaudio) {
        throw new std::runtime_error("Attempt to get stream info while not initialized");
    }

    if (!_streamInfo) {
        _streamInfo = std::shared_ptr<StreamInfo>(new StreamInfo());
        _streamInfo->isOpen = _rtaudio->isStreamOpen();
        _streamInfo->isRunning = _rtaudio->isStreamRunning();
        _streamInfo->latency = _rtaudio->getStreamLatency();
        _streamInfo->time = _rtaudio->getStreamTime();
        _streamInfo->sampleRate = _rtaudio->getStreamSampleRate();
    }

    return _streamInfo.get();
}

double DriverProvider::time() const
{
    if (!_rtaudio) {
        return 0;
    }

    return _rtaudio->getStreamTime();
}

void DriverProvider::error(RtAudioError::Type type, const std::string &errorText)
{
    if (type == RtAudioError::DEBUG_WARNING || type == RtAudioError::WARNING) {
        std::cerr << errorText.c_str() << "\n";
    } else {
        throw std::runtime_error(errorText);
    }
}

int DriverProvider::process(void *output, void *input, unsigned frames,
                            double time, RtAudioStreamStatus status,
                            void *data)
{
    auto pthis = reinterpret_cast<DriverProvider *>(data);

    if (pthis->_process) {
        const StreamInfo *info = pthis->streamInfo();
        ProcessParams params = {
            info->input.channels,
            info->output.channels,
            frames,
            input,
            info->latency,
            output,
            info->sampleRate,
            status,
            time,
        };

        return (*pthis->_process)(params);
    }

    return 0;
}
