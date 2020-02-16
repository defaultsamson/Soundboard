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
    int volumeInt;
    float volume;
    bool isInput;
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
    DeviceInfoContainer *defaultOutput();
    DeviceInfoContainer *defaultInput();

    void addActiveDevice(DeviceInfoContainer*);
    void removeActiveDevice(DeviceInfoContainer*);

    void removeActiveDisplayOutput(int deviceDisplayIndex);
    DeviceInfoContainer* getActiveDisplayOutput(int deviceDisplayIndex);
    const QList<DeviceInfoContainer*> activeOutputs();

    void removeActiveDisplayInput(int deviceDisplayIndex);
    DeviceInfoContainer* getActiveDisplayInput(int deviceDisplayIndex);
    const QList<DeviceInfoContainer*> activeInputs();

    const QList<HostInfoContainer*> hosts();
    const QList<DeviceInfoContainer*> devices();

    void refreshDevices();

    void init();
    bool isInitialized();

    static int outputCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    static int inputCallback(const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData);

    void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);

    void registerAudio(AudioObject *);
    void unregisterAudio(AudioObject *);

    ~AudioEngine();

private:
    Main *main;
    bool _isInitialized = false;

    HostInfoContainer *_defaultOutputHost = nullptr;
    DeviceInfoContainer *_defaultOutput = nullptr;
    HostInfoContainer *_defaultInputHost = nullptr;
    DeviceInfoContainer *_defaultInput = nullptr;
    QList<HostInfoContainer*> _hosts;
    QList<DeviceInfoContainer*> _devices;
    QList<DeviceInfoContainer*> _activeOutputs;
    QList<DeviceInfoContainer*> _activeInputs;
    QList<DeviceIndexInfo> _selectedDeviceIndexes;
    QList<AudioObject *> _audioObjectRegistry;

    DeviceInfoContainer *getDevice(int deviceIndex);

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
