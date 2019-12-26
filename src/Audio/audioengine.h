#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QList>
#include <QAudioDeviceInfo>

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
