#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>
#ifdef Q_OS_LINUX
#include <pa_jack.h>
#endif
#include "../mainapp.h"
#include <memory>
#include <iostream>

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
size_t AudioEngine::FRAMES_PER_BUFFER = 256;
size_t AudioEngine::CHANNELS = 2;

AudioEngine::AudioEngine(Main* _main) : _main(_main) {
}
AudioEngine::~AudioEngine() {
    Pa_Terminate();
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
    delete _inputObject;
}
Device* AudioEngine::defaultOutput() {
    return _defaultOutput;
}
Device* AudioEngine::defaultInput() {
    return _defaultInput;
}

void AudioEngine::addActiveOutput(Device *device) {
    addActiveDevice(device, false, true);
}
void AudioEngine::addActiveInput(Device *device) {
    addActiveDevice(device, true, false);
}
void AudioEngine::removeActiveOutput(Device* device) {
    removeActiveDevice(device, false, true);
}
void AudioEngine::removeActiveInput(Device* device) {
    removeActiveDevice(device, true, false);
}

void AudioEngine::addActiveDevice(Device* dev, bool startInput, bool startOutput) {
    if (!dev) return;

    if (dev->stream) {
        Pa_CloseStream(dev->stream);
        dev->stream = nullptr;
    }

    if (!startInput && !startOutput) return;

    startInput = startInput || dev->isInputting();
    startOutput = startOutput || dev->isOutputting();

    // First stop both devices (we will start them again, but using the same stream)
    if (dev->isOutputting()) _activeOutputs.removeOne(dev);
    if (dev->isInputting()) _activeInputs.removeOne(dev);

    // Adding device to the end of the list
    if (startOutput) {
        dev->indexes()->outputListIndex = _activeOutputs.size();
        _activeOutputs.append(dev);
        _selectedOutputIndexes.append(dev->indexes());
    }
    if (startInput) {
        dev->indexes()->inputListIndex = _activeInputs.size();
        _activeInputs.append(dev);
        _selectedInputIndexes.append(dev->indexes());
    }
    // size_t channels = CHANNELS <= device->info->maxOutputChannels ? CHANNELS : device->info->maxOutputChannels;

    PaError err;
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
    if (startOutput) {
        outputParameters.device = dev->indexes()->deviceIndex;
        outputParameters.channelCount = static_cast<int>(CHANNELS); // device.info->maxOutputChannels;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = dev->info()->defaultLowOutputLatency ;
        outputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field
    }
    if (startInput) {
        inputParameters.device = dev->indexes()->deviceIndex;
        inputParameters.channelCount = static_cast<int>(CHANNELS); // device.info->maxOutputChannels;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = dev->info()->defaultLowOutputLatency ;
        inputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field
    }

    err = Pa_OpenStream(
                    &dev->stream,
                    startInput ? &inputParameters : nullptr,
                    startOutput ? &outputParameters : nullptr,
                    dev->info()->defaultSampleRate,
                    FRAMES_PER_BUFFER,
                    paNoFlag, //flags that can be used to define dither, clip settings and more
                    &AudioEngine::audioCallback, //your callback function
                    static_cast<CallbackInfo*>(new CallbackInfo{this, dev})); //data to be passed to callback. In C++, it is frequently (void *)this
    //don't forget to check errors!
    if (err != paNoError) std::cout << "Error opening stream" << std::endl;
    // else std::cout << "Stream opened successfully!" << std::endl;

    err = Pa_StartStream(dev->stream);
    if (err != paNoError) std::cout << "Error starting stream" << std::endl;
    // else std::cout << "Stream started successfully!" << std::endl;

    dev->setInputting(startInput);
    dev->setOutputting(startOutput);

    _inputObject->setHasInputDevice(_activeInputs.size() > 0);
}
void AudioEngine::removeActiveDevice(Device* dev, bool stopInput, bool stopOutput) {
    if (!dev) return;

    stopInput = stopInput && dev->isInputting();
    stopOutput = stopOutput && dev->isOutputting();

    if (!stopInput && !stopOutput) return;

    if (stopInput) {
        dev->indexes()->inputDisplayIndex = -1;
        // Remove its info so upon reaload or refresh it doesn't select the device again
        for (int i = 0; i < _selectedInputIndexes.size(); i++) {
            if (_selectedInputIndexes.at(i)->deviceIndex == dev->indexes()->deviceIndex) {
                _selectedInputIndexes.removeAt(i);
                break;
            }
        }
        _activeInputs.removeOne(dev);
        // Updates the deviceListIndex data
        for (int i = 0; i < _activeInputs.size(); i++) {
            _activeInputs.at(i)->indexes()->inputListIndex = i;
        }
    }
    if (stopOutput) {
        dev->indexes()->outputDisplayIndex = -1;
        // Remove its info so upon reaload or refresh it doesn't select the device again
        for (int i = 0; i < _selectedOutputIndexes.size(); i++) {
            if (_selectedOutputIndexes.at(i)->deviceIndex == dev->indexes()->deviceIndex) {
                _selectedOutputIndexes.removeAt(i);
                break;
            }
        }
        _activeOutputs.removeOne(dev);
        // Updates the deviceListIndex data
        for (int i = 0; i < _activeOutputs.size(); i++) {
            _activeOutputs.at(i)->indexes()->outputListIndex = i;
        }
    }

    if (stopInput) dev->setInputting(false);
    if (stopOutput) dev->setOutputting(false);

    // This closes any streams and restarts any that are neccessary
    // e.g. if the output was stopped but this device is still inputting
    addActiveDevice(dev, dev->isInputting(), dev->isOutputting());
}

void AudioEngine::removeActiveDisplayOutput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeOutputs) if (dev->indexes()->outputDisplayIndex == deviceDisplayIndex) { removeActiveOutput(dev); break; }
}
Device* AudioEngine::getActiveDisplayOutput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeOutputs) if (dev->indexes()->outputDisplayIndex == deviceDisplayIndex) return dev;
    return nullptr;
}
const QList<Device*> AudioEngine::activeOutputs() {
    return _activeOutputs;
}

void AudioEngine::removeActiveDisplayInput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeInputs) if (dev->indexes()->inputDisplayIndex == deviceDisplayIndex) { removeActiveInput(dev); break; }
}
Device* AudioEngine::getActiveDisplayInput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeInputs) if (dev->indexes()->inputDisplayIndex == deviceDisplayIndex) return dev;
    return nullptr;
}
const QList<Device*> AudioEngine::activeInputs() {
    return _activeInputs;
}

const QList<HostInfoContainer*> AudioEngine::hosts() {
    return _hosts;
}
const QList<Device*> AudioEngine::outputs() {
    return _outputs;
}
const QList<Device*> AudioEngine::inputs() {
    return _inputs;
}

void AudioEngine::init() {
    _main->showAudioEngineText(true);
#ifdef Q_OS_LINUX
    PaJack_SetClientName("Soundboard");
#endif
    Pa_Initialize();
    _inputObject = new AudioObjectInput();
    _inputObject->setOutput0(_main->settings()->value(Main::INPUT_OUT0, false).toBool());
    _inputObject->setOutput1(_main->settings()->value(Main::INPUT_OUT1, false).toBool());
    refreshDevices();
    _isInitialized = true;
}

bool AudioEngine::isInitialized() {
    return _isInitialized;
}

void AudioEngine::refreshDevices() {
    _main->showAudioEngineText(true);

    int devices = Pa_GetDeviceCount();
    std::cout << "Refreshing devices... [" << devices << "]" << std::endl;

    _defaultOutput = nullptr;
    _defaultInput = nullptr;
    _defaultHost = nullptr;
    for (auto dev : _activeOutputs) Pa_CloseStream(dev->stream);
    for (auto dev : _activeInputs) Pa_CloseStream(dev->stream);
    _activeOutputs.clear();
    _activeInputs.clear();
    _outputs.clear();
    _inputs.clear();
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
    _hosts.clear();

    const PaDeviceInfo* device;

    for (int i = 0; i < devices; ++i ) {
        device = Pa_GetDeviceInfo(i);
        bool isInput = device->maxInputChannels > 0;
        bool isOutput = device->maxOutputChannels > 0;
        Device* dev = new Device{nullptr, device, CHANNELS, std::shared_ptr<DeviceIndexInfo>(new DeviceIndexInfo{i, -1, -1, -1, -1}), isInput, isOutput};

        if (isInput) {
            _inputs.append(dev);
            if (Pa_GetDefaultInputDevice() == i) {
                _defaultInput = dev;
                std::cout << "Default input device... [" << i << "]" << std::endl;
            } else
                std::cout << "Input device... [" << i << "]" << std::endl;
        }
        if (isOutput) {
            _outputs.append(dev);
            if (Pa_GetDefaultOutputDevice() == i) {
                _defaultOutput = dev;
                std::cout << "Default output device... [" << i << "]" << std::endl;
            }else
                std::cout << "Output device... [" << i << "]" << std::endl;
        }

        // Try to find the container for the specific host that contains all its devices
        bool foundCon = false;
        for (auto hostCon : _hosts) {
            if (hostCon->index == device->hostApi) {
                foundCon = true;
                dev->setHost(hostCon);
                hostCon->devices->append(dev);
                break;
            }
        }
        // Create the container if not found
        if (!foundCon) {
            QList<Device*> *list = new QList<Device*>();
            list->append(dev);
            HostInfoContainer* hostCon = new HostInfoContainer{
                device->hostApi,
                Pa_GetHostApiInfo(device->hostApi)->name,
                list
            };
            dev->setHost(hostCon);
            _hosts.append(hostCon);

            if (Pa_GetDefaultHostApi() == device->hostApi)
                _defaultHost = hostCon;
        }
    }

    // Devices are loaded, now determine which are supposed to be active
    Device* out0 = getDevice(_main->settings()->value(Main::OUTPUT_INDEX0, -1).toInt());
    Device* out1 = getDevice(_main->settings()->value(Main::OUTPUT_INDEX1, -1).toInt());
    Device* in0 = getDevice(_main->settings()->value(Main::INPUT_INDEX0, -1).toInt());
    _inputObject->setMute(_main->settings()->value(Main::INPUT_MUTED, false).toBool());

    // There's a saved device, load it
    if (out0) {
        out0->indexes()->outputDisplayIndex = 0;
        out0->setVolume(_main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt());
        addActiveOutput(out0);
    }
    if (out1) {
        out1->indexes()->outputDisplayIndex = 1;
        out1->setVolume(_main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt());
        addActiveOutput(out1);
    }
    if (in0) {
        in0->indexes()->inputDisplayIndex = 0;
        _inputObject->setVolumeInt(_main->settings()->value(Main::INPUT_VOLUME0, 100).toInt());
        addActiveInput(in0);
    }

    // If no device was found, and the devices weren't explicitly all removed, load the defaults
    if (_activeOutputs.size() == 0 && _defaultOutput && !_main->settings()->value(Main::EXPLICIT_NO_OUTPUT_DEVICES, false).toBool()) {
        _main->settings()->setValue(Main::OUTPUT_INDEX0, _defaultOutput ? _defaultOutput->indexes()->deviceIndex : -1);
        _defaultOutput->indexes()->outputDisplayIndex = 0;
        addActiveOutput(_defaultOutput);
    }
    if (_activeInputs.size() == 0 && _defaultInput && !_main->settings()->value(Main::EXPLICIT_NO_INPUT_DEVICES, false).toBool()) {
        _main->settings()->setValue(Main::INPUT_INDEX0, _defaultInput ? _defaultInput->indexes()->deviceIndex : -1);
        _defaultInput->indexes()->inputDisplayIndex = 0;
        addActiveInput(_defaultInput);
    }

    _main->showAudioEngineText(false);
}

Device* AudioEngine::getDevice(int deviceIndex) {
    for (auto dev : _outputs)
        if (dev->indexes()->deviceIndex == deviceIndex)
            return dev;

    for (auto dev : _inputs)
        if (dev->indexes()->deviceIndex == deviceIndex)
            return dev;

    return nullptr;
}

void AudioEngine::registerAudio(AudioObjectFile* obj) {
    if (!_audioObjectRegistry.contains(obj)) _audioObjectRegistry.append(obj);
}
void AudioEngine::unregisterAudio(AudioObjectFile* obj) {
    if (_audioObjectRegistry.contains(obj)) _audioObjectRegistry.removeOne(obj);
}

// Mixes audio from all the AudioObjects
void AudioEngine::mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice) {

    size_t frames = framesPerBuffer * channels;

    // Fills the buffer with zeros
    memset(buffer, 0, frames * sizeof(float));

    for (AudioObject* audio : _audioObjectRegistry) {
        audio->mix(buffer, framesPerBuffer, channels, deviceListIndex, deviceVolume, singleDevice);
    }
}

int AudioEngine::audioCallback(const void* inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {

    CallbackInfo* info = static_cast<CallbackInfo*>(userData);
    AudioEngine* a = info->audio;
    AudioObjectInput* input = a->inputObject();
    Device* dev = info->device;
    std::shared_ptr<DeviceIndexInfo> indexes = dev->indexes();

    if (dev->isInputting()) {
        const float* in = static_cast<const float*>(inputBuffer);
        // std::cout << in[0] << std::endl;
        CallbackInfo* info = static_cast<CallbackInfo*>(userData);
        info->audio->inputObject()->write(in, framesPerBuffer * CHANNELS);

    }

    if (dev->isOutputting()) {
        float* out = static_cast<float*>(outputBuffer);
        a->mix(out, framesPerBuffer, CHANNELS, indexes->outputListIndex, dev->volume(), a->activeOutputs().size() == 1);

        // Mix from the InputObject, if this device is set to output it
        if (input->hasInputDevice()
                && ((input->isActiveOutput0() && indexes->outputDisplayIndex == 0)
                    || (input->isActiveOutput1() && indexes->outputDisplayIndex == 1))) {
            bool single = (input->isActiveOutput0() && !input->isActiveOutput1()) || (!input->isActiveOutput0() && input->isActiveOutput1());
            a->inputObject()->mix(out, framesPerBuffer, CHANNELS, indexes->outputListIndex, dev->volume(), single);
        }
    }
    return paContinue;
}

void AudioEngine::updateSavedDevices() {
    Device* dev;
    dev = getActiveDisplayOutput(0);
    _main->settings()->setValue(Main::OUTPUT_INDEX0, dev ? dev->indexes()->deviceIndex : -1);
    dev = getActiveDisplayOutput(1);
    _main->settings()->setValue(Main::OUTPUT_INDEX1, dev ? dev->indexes()->deviceIndex : -1);
    dev = getActiveDisplayInput(0);
    _main->settings()->setValue(Main::INPUT_INDEX0, dev ? dev->indexes()->deviceIndex : -1);

    _main->settings()->setValue(Main::EXPLICIT_NO_OUTPUT_DEVICES, _activeOutputs.count() == 0);
    _main->settings()->setValue(Main::EXPLICIT_NO_INPUT_DEVICES, _activeInputs.count() == 0);
}
