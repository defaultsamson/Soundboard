#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QList>
#include <QAudioDeviceInfo>

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

    QList<QAudioDeviceInfo> devices();
    QList<QAudioDeviceInfo> activeDevices();
    void refreshDevices();
private:
    QList<QAudioDeviceInfo> _devices;
    QList<QAudioDeviceInfo> _activeDevices;
};

#endif // AUDIOENGINE_H
