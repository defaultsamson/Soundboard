#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

#include <portaudio.h>

// Forward declarations
struct HostInfoContainer;
struct DeviceInfoContainer;
class AudioEngine;

struct CallbackInfo {
    AudioEngine *audio;
    DeviceInfoContainer *device;
};

struct DeviceIndexInfo {
    PaDeviceIndex deviceIndex;
    int displayIndex;
    int deviceListIndex;
};

struct DeviceInfoContainer {
    HostInfoContainer *host;
    const PaDeviceInfo *info;
    PaStream *stream;
    size_t channels;
    DeviceIndexInfo indexes;
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
    AudioEngine(Main *main);

    static size_t FRAMES_PER_BUFFER;
    static size_t CHANNELS;

    HostInfoContainer *defaultHost();
    DeviceInfoContainer *defaultDevice();

    void addActiveDevice(DeviceInfoContainer*);
    void removeActiveDevice(DeviceInfoContainer*);
    void removeActiveDevice(int deviceDisplayIndex);
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

    void mix(float* buffer, size_t frames, int deviceListIndex);

    void registerAudio(AudioObject *);
    void unregisterAudio(AudioObject *);

    ~AudioEngine();

private:
    Main *main;
    bool _isInitialized = false;

    HostInfoContainer *_defaultHost = nullptr;
    DeviceInfoContainer *_defaultDevice = nullptr;
    QList<HostInfoContainer*> _hosts;
    QList<DeviceInfoContainer*> _devices;
    QList<DeviceInfoContainer*> _activeDevices;
    QList<DeviceIndexInfo> _selectedDeviceIndexes;
    QList<AudioObject *> _audioObjectRegistry;

    DeviceInfoContainer *getDevice(int deviceIndex);

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
