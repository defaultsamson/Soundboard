#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

#include <portaudio.h>

struct HostInfoContainer {
    PaHostApiIndex hostIndex;
    const char* hostName;
    QList <DeviceInfoContainer*> *devices;

    ~HostInfoContainer() {
        if (devices) {
            for (int i = 0; i < devices->size(); i++) delete devices->at(i);
            delete devices;
        }
    }
};

class AudioEngine : public QObject
{
    Q_OBJECT

public:
    AudioEngine();

    HostInfoContainer *defaultHost();
    DeviceInfoContainer *defaultDevice();
    void setSelectedHost(HostInfoContainer*);
    HostInfoContainer *selectedHost();
    void setSelectedDevice(DeviceInfoContainer*);
    DeviceInfoContainer *selectedDevice();
    QList<HostInfoContainer*> hosts();
    void refreshDevices();

    HostInfoContainer *activeHost();
    DeviceInfoContainer *activeDevice();

    void init();
    bool isInitialized();

    static int readCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    void mix(float* buffer, size_t frames);

    void registerAudio(AudioObject *);
    void unregisterAudio(AudioObject *);

    ~AudioEngine();

private:
    PaStream *stream = nullptr;
    int channels = 0;
    PaHostApiIndex selectedDeviceIndex = -1;

    bool _isInitialized = false;

    HostInfoContainer *_defaultHost = nullptr;
    DeviceInfoContainer *_defaultDevice = nullptr;
    HostInfoContainer *_selectedHost = nullptr;
    DeviceInfoContainer *_selectedDevice = nullptr;
    QList<HostInfoContainer*> _hosts;

    QList<AudioObject *> _audioObjectRegistry;

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
