#ifndef DRIVERRTAUDIO_H
#define DRIVERRTAUDIO_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include <RtAudio.h>

#include "shared/idriver.h"

class DriverRtAudio : public IDriver {
public:
    DriverRtAudio();
    virtual ~DriverRtAudio();

    const Api* apiInfo(ApiType type);
    unsigned connect(const ConnectOptions& options);
    void control(Control command);
    void disconnect();
    const DriverInfo* driverInfo();
    const StreamInfo* streamInfo();
    double time() const;

protected:
    void setProcess(Process callback, void* object);

private:
    std::map<ApiType, std::shared_ptr<Api> > _apiInfos;
    static std::string _names[ApiCount];
    std::shared_ptr<DriverInfo> _driverInfo;
    std::string _name;
    Process _process;
    void* _processObject;
    std::shared_ptr<RtAudio> _rtaudio;
    std::shared_ptr<StreamInfo> _streamInfo;

    static void error(RtAudioError::Type type, const std::string& errorText);
    static int process(void* output, void* input, unsigned frames,
        double time, RtAudioStreamStatus status, void* data);
};

#endif // DRIVERRTAUDIO_H
