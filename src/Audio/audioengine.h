#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "audioobjectinput.h"
#include "../Widget/listitemsound.h"

#include "device.h"
#include <memory>

struct CallbackInfo {
    AudioEngine* audio;
    Device* device;
};

class AudioEngine : public QObject
{
    Q_OBJECT

public:
    AudioEngine(Main* main);

    static size_t FRAMES_PER_BUFFER;
    static size_t CHANNELS;

    Device* defaultOutput();
    Device* defaultInput();

    void addActiveOutput(Device*);
    void removeActiveOutput(Device*);
    void addActiveInput(Device*);
    void removeActiveInput(Device*);

    void removeActiveDisplayOutput(int deviceDisplayIndex);
    Device* getActiveDisplayOutput(int deviceDisplayIndex);
    const QList<Device*> activeOutputs();

    void removeActiveDisplayInput(int deviceDisplayIndex);
    Device* getActiveDisplayInput(int deviceDisplayIndex);
    const QList<Device*> activeInputs();

    const QList<HostInfoContainer*> hosts();
    const QList<Device*> outputs();
    const QList<Device*> inputs();

    void refreshDevices();

    void init();
    bool isInitialized();

    static int audioCallback(const void* inputBuffer, void* outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void* userData);

    void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);

    void registerAudio(AudioObject*);
    void unregisterAudio(AudioObject*);

    AudioObjectInput* inputObject() { return _inputObject; }

    ~AudioEngine();

private:
    Main* main;
    bool _isInitialized = false;

    Device* _defaultOutput = nullptr;
    Device* _defaultInput = nullptr;
    QList<HostInfoContainer*> _hosts;
    QList<Device*> _outputs;
    QList<Device*> _inputs;
    QList<Device*> _activeOutputs;
    QList<Device*> _activeInputs;
    QList<std::shared_ptr<DeviceIndexInfo>> _selectedOutputIndexes;
    QList<std::shared_ptr<DeviceIndexInfo>> _selectedInputIndexes;
    QList<AudioObject*> _audioObjectRegistry;
    AudioObjectInput* _inputObject = nullptr;

    Device* getDevice(int deviceIndex);
    void addActiveDevice(Device*, bool startInput, bool startOutput);
    void removeActiveDevice(Device*, bool stopInput, bool stopOutput);

signals:
    void update(qreal m_level);
};

#endif // AUDIOENGINE_H
