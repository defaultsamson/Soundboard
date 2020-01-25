#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

#include <portaudio.h>

// Hotkey-Audio Connector Object
struct HACObj {
    ListItemSound* listItem;
    AudioObject* audio;
};

class AudioEngine
{
public:
    AudioEngine();

    QList<const PaDeviceInfo*> devices();
    QList<const PaDeviceInfo*> activeDevices();
    void refreshDevices();
private:
    QList<const PaDeviceInfo*> _devices;
    QList<const PaDeviceInfo*> _activeDevices;
};

#endif // AUDIOENGINE_H
