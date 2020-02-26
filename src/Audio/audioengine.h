#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QList>

#include "audioobject.h"
#include "audioobjectinput.h"
#include "audioobjectfile.h"
#include "../Widget/listitemsound.h"

#include "device.h"
#include <memory>

struct CallbackInfo {
    AudioEngine* audio;
    Device* device;
};

class AudioEngine
{
public:
    AudioEngine(Main* _main);

    static size_t FRAMES_PER_BUFFER;
    static size_t CHANNELS;

    HostInfoContainer* defaultHost() { return _defaultHost; }
    Device* defaultOutput();
    Device* defaultInput();

    void addActiveOutput(Device*);
    void removeActiveOutput(Device*);
    void addActiveInput(Device*);
    void removeActiveInput(Device*);
    void addActiveDevice(Device*, bool startInput, bool startOutput);
    void removeActiveDevice(Device*, bool stopInput, bool stopOutput);

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

    void registerAudio(AudioObjectFile*);
    void unregisterAudio(AudioObjectFile*);
    const QList<AudioObjectFile*> audioRegistry() { return _audioObjectRegistry; }

    AudioObjectInput* inputObject() { return _inputObject; }

    void updateSavedDevices();

    ~AudioEngine();

private:
    Main* _main;
    bool _isInitialized = false;

    Device* _defaultOutput = nullptr;
    Device* _defaultInput = nullptr;
    HostInfoContainer* _defaultHost = nullptr;
    QList<HostInfoContainer*> _hosts;
    QList<Device*> _outputs;
    QList<Device*> _inputs;
    QList<Device*> _activeOutputs;
    QList<Device*> _activeInputs;
    QList<std::shared_ptr<DeviceIndexInfo>> _selectedOutputIndexes;
    QList<std::shared_ptr<DeviceIndexInfo>> _selectedInputIndexes;
    QList<AudioObjectFile*> _audioObjectRegistry;
    AudioObjectInput* _inputObject = nullptr;

    void updateListIndexes(bool inputs, bool outputs);

    Device* getDevice(int deviceIndex);
};

#endif // AUDIOENGINE_H
