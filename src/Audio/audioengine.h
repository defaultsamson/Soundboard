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

class AudioEngine
{
public:
    AudioEngine();

    QList<DeviceInfoContainer> devices();
    QList<DeviceInfoContainer> activeDevices();
    void refreshDevices();
private:
    QList<DeviceInfoContainer> _devices;
    QList<DeviceInfoContainer> _activeDevices;
};

#endif // AUDIOENGINE_H
