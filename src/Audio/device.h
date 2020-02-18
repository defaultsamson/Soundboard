#ifndef DEVICE_H
#define DEVICE_H

#include <portaudio.h>
#include <QList>
#include <memory>

// Forward declarations
class Device;
class AudioEngine;
struct HostInfoContainer;

struct CallbackInfo {
    AudioEngine* audio;
    Device* device;
};

struct DeviceIndexInfo {
    PaDeviceIndex deviceIndex;
    int outputDisplayIndex;
    int inputDisplayIndex;
    int outputListIndex;
    int inputListIndex;
};

class Device
{
public:
    Device(HostInfoContainer* host,
           const PaDeviceInfo* info,
           size_t channels,
           std::shared_ptr<DeviceIndexInfo> indexes,
           bool isInput,
           bool isOutput);

    int volumeInt() { return _volumeInt; }
    float volume() { return _volume; }
    void setVolume(int volumeInt);

    HostInfoContainer* host() { return _host; }
    void setHost(HostInfoContainer* host) { _host = host; }
    const PaDeviceInfo* info() { return _info; }
    size_t channels() { return _channels; }
    std::shared_ptr<DeviceIndexInfo> indexes() { return _indexes; }
    bool isInput() { return _isInput; }
    bool isOutput() { return _isOutput; }
    bool isInputting() { return _isInputting; }
    bool isOutputting() { return _isOutputting; }
    void setInputting(bool inputting) { _isInputting = inputting; }
    void setOutputting(bool outputting) { _isOutputting = outputting; }

    PaStream* stream;

private:
    HostInfoContainer* _host;
    const PaDeviceInfo* _info;
    size_t _channels;
    std::shared_ptr<DeviceIndexInfo> _indexes;
    bool _isInput;
    bool _isOutput;

    int _volumeInt = 100;
    float _volume = 1;
    bool _isInputting = false;
    bool _isOutputting = false;
};

struct HostInfoContainer {
    PaHostApiIndex index;
    const char* name;
    QList <Device*>* devices;

    ~HostInfoContainer() {
        if (devices) {
            for (int i = 0; i < devices->size(); i++) delete devices->at(i);
            delete devices;
        }
    }
};

#endif // DEVICE_H
