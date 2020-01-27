#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "../Widget/listitemsound.h"

#include <portaudio.h>

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

class AudioEngine : public QObject
{
    Q_OBJECT

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

    static int readCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    void mix(float* buffer, size_t frames);

    void registerAudio(AudioObject *);
    void unregisterAudio(AudioObject *);

private:
    PaStream *stream = nullptr;
    int channels = 0;

    bool _hasDefaultHost = false;
    HostInfoContainer _defaultHost;
    bool _hasDefaultDevice = false;
    DeviceInfoContainer _defaultDevice;
    bool _hasSelectedHost = false;
    HostInfoContainer _selectedHost;
    bool _hasSelectedDevice = false;
    DeviceInfoContainer _selectedDevice;
    QList<HostInfoContainer> _hosts;

    QList<AudioObject *> _audioObjectRegistry;

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
