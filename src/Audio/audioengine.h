#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

// Hotkey-Audio Connector Object
struct HACObj {
    ListItemSound* listItem;
    AudioObject* audio;
};

struct HostInfoContainer {
    PaHostApiIndex hostIndex;
    const char* hostName;
    QList <DeviceInfoContainer> *devices;
};

class AudioEngine
{
public:
    AudioEngine();

    bool hasDefaultHost();
    HostInfoContainer defaultHost();
    bool hasDefaultDevice();
    DeviceInfoContainer defaultDevice();
    bool hasSelectedHost();
    void setSelectedHost(HostInfoContainer);
    HostInfoContainer selectedHost();
    bool hasSelectedDevice();
    void setSelectedDevice(DeviceInfoContainer);
    DeviceInfoContainer selectedDevice();
    QList<HostInfoContainer> hosts();
    void refreshDevices();

    bool hasActiveHost();
    HostInfoContainer activeHost();
    bool hasActiveDevice();
    DeviceInfoContainer activeDevice();

    void init();
private:
    bool _hasDefaultHost = false;
    HostInfoContainer _defaultHost;
    bool _hasDefaultDevice = false;
    DeviceInfoContainer _defaultDevice;
    bool _hasSelectedHost = false;
    HostInfoContainer _selectedHost;
    bool _hasSelectedDevice = false;
    DeviceInfoContainer _selectedDevice;
    QList<HostInfoContainer> _hosts;
};

#endif // AUDIOENGINE_H
