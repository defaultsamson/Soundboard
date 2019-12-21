#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QList>
#include <QAudioDeviceInfo>

class AudioEngine
{
public:
    AudioEngine();

    QList<QAudioDeviceInfo> devices();
    void refreshDevices();
private:
    QList<QAudioDeviceInfo> _devices;
};

#endif // AUDIOENGINE_H
