#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

#include <portaudio.h>

struct HostInfoContainer;

struct DeviceInfoContainer {
    HostInfoContainer *host;
    const PaDeviceInfo *info;
    PaDeviceIndex index;
    PaStream *stream;
    int channels;
};

struct HostInfoContainer {
    PaHostApiIndex index;
    const char* name;
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

    void addActiveHost(HostInfoContainer*);
    void removeActiveHost(HostInfoContainer*);
    const QList<HostInfoContainer*> activeHosts();

    void addActiveDevice(DeviceInfoContainer*);
    void removeActiveDevice(DeviceInfoContainer*);
    const QList<DeviceInfoContainer*> activeDevices();

    const QList<HostInfoContainer*> hosts();
    const QList<DeviceInfoContainer*> devices();

    void refreshDevices();

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
    PaHostApiIndex selectedDeviceIndex = -1;

    bool _isInitialized = false;

    HostInfoContainer *_defaultHost = nullptr;
    DeviceInfoContainer *_defaultDevice = nullptr;
    QList<HostInfoContainer*> _hosts;
    QList<DeviceInfoContainer*> _devices;
    QList<HostInfoContainer*> _activeHosts;
    QList<DeviceInfoContainer*> _activeDevices;

    QList<AudioObject *> _audioObjectRegistry;

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
