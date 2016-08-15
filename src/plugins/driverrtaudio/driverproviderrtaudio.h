#ifndef DRIVERPROVIDERRTAUDIO_H
#define DRIVERPROVIDERRTAUDIO_H

#include <string>
#include <list>
#include <map>
#include <memory>

#include <RtAudio.h>

#include "shared/idriver.h"

class DriverProviderRtAudio: public IDriver
{
public:
    DriverProviderRtAudio();
    virtual ~DriverProviderRtAudio();

    const Api *apiInfo(ApiType type);
    unsigned connect(const ConnectOptions &options);
    void control(Control command);
    void disconnect();
    const DriverInfo *driverInfo();
    void setProcess(Process callback);
    const StreamInfo *streamInfo();
    double time() const;

private:
    std::map<ApiType, std::shared_ptr<Api>> _apiInfos;
    static std::string _names[ApiCount];
    std::shared_ptr<DriverInfo> _driverInfo;
    std::string _name;
    Process _process;
    std::shared_ptr<RtAudio> _rtaudio;
    std::shared_ptr<StreamInfo> _streamInfo;

    static void error(RtAudioError::Type type, const std::string &errorText);
    static int process(void *output, void *input, unsigned frames,
                       double time, RtAudioStreamStatus status, void *data);
};

#endif // DRIVERPROVIDERRTAUDIO_H
