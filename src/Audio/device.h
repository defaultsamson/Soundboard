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
    int displayIndex;
    int deviceListIndex;
};

class Device
{
public:
    Device(HostInfoContainer* host,
           const PaDeviceInfo* info,
           size_t channels,
           std::shared_ptr<DeviceIndexInfo> indexes,
           bool isInput);

    int volumeInt() { return _volumeInt; }
    float volume() { return _volume; }
    void setVolume(int volumeInt);

    HostInfoContainer* host() { return _host; }
    void setHost(HostInfoContainer* host) { _host = host; }
    const PaDeviceInfo* info() { return _info; }
    size_t channels() { return _channels; }
    std::shared_ptr<DeviceIndexInfo> indexes() { return _indexes; }
    bool isInput() { return _isInput; }

    PaStream* stream;

private:
    HostInfoContainer* _host;
    const PaDeviceInfo* _info;
    size_t _channels;
    std::shared_ptr<DeviceIndexInfo> _indexes;
    bool _isInput;

    int _volumeInt = 100;
    float _volume = 1;
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
