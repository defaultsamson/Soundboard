#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>
#include "../mainapp.h"
#include <memory>

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
size_t AudioEngine::FRAMES_PER_BUFFER = 256;
size_t AudioEngine::CHANNELS = 2;

AudioEngine::AudioEngine(Main* main) : main(main) {
}
AudioEngine::~AudioEngine() {
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
                    &AudioEngine::outputCallback, //your callback function
                    static_cast<CallbackInfo*>(new CallbackInfo{this, dev})); //data to be passed to callback. In C++, it is frequently (void *)this
    //don't forget to check errors!
    if (err != paNoError) qDebug() << "Error opening stream";
    // else qDebug() << "Stream opened successfully!";

    err = Pa_StartStream(dev->stream);
    if (err != paNoError) qDebug() << "Error starting stream";
    // else qDebug() << "Stream started successfully!";

    dev->setInputting(startInput);
    dev->setOutputting(startOutput);
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

    // This closes any streams and restarts any that are neccessary
    // e.g. if the output was stopped but this device is still inputting
    addActiveDevice(dev, !stopInput && dev->isInputting(), !stopOutput && dev->isOutputting());
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
    Pa_Initialize();
    _inputObject = new AudioObjectInput();
    _inputObject->setOutput0(main->settings()->value(Main::INPUT_OUT0, false).toBool());
    _inputObject->setOutput1(main->settings()->value(Main::INPUT_OUT1, false).toBool());
    refreshDevices();
    _isInitialized = true;
}

bool AudioEngine::isInitialized() {
    return _isInitialized;
}

void AudioEngine::refreshDevices() {

    int devices = Pa_GetDeviceCount();
    qDebug() << "Refreshing devices... [" << devices << "]";

    _defaultOutput = nullptr;
    _defaultInput = nullptr;
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
        Device* dev = dev = new Device{nullptr, device, CHANNELS, std::shared_ptr<DeviceIndexInfo>(new DeviceIndexInfo{i, -1, -1}), isInput, isOutput};

        if (isInput) {
            _inputs.append(dev);
            if (Pa_GetDefaultInputDevice() == i) {
                _defaultInput = dev;
                qDebug() << "Default input device... [" << i << "]";
            } else
                qDebug() << "Input device... [" << i << "]";
        }
        if (isOutput) {
            _outputs.append(dev);
            if (Pa_GetDefaultOutputDevice() == i) {
                _defaultOutput = dev;
                qDebug() << "Default output device... [" << i << "]";
            }else
                qDebug() << "Output device... [" << i << "]";
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
        }
    }

    // Devices are loaded, now determine which are supposed to be active
    Device* out0 = getDevice(main->settings()->value(Main::OUTPUT_INDEX0, -1).toInt());
    Device* out1 = getDevice(main->settings()->value(Main::OUTPUT_INDEX1, -1).toInt());
    Device* in0 = getDevice(main->settings()->value(Main::INPUT_INDEX0, -1).toInt());

    // There's a saved device, load it
    if (out0) {
        out0->indexes()->outputDisplayIndex = 0;
        out0->setVolume(main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt());
        addActiveOutput(out0);
    }
    if (out1) {
        out1->indexes()->outputDisplayIndex = 1;
        out1->setVolume(main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt());
        addActiveOutput(out1);
    }
    if (in0) {
        in0->indexes()->inputDisplayIndex = 0;
        in0->setVolume(main->settings()->value(Main::INPUT_VOLUME0, 100).toInt());
        addActiveInput(in0);
    }

    // If no device was found, and the devices weren't explicitly all removed, load the defaults
    if (_activeOutputs.size() == 0 && _defaultOutput && !main->settings()->value(Main::EXPLICIT_NO_OUTPUT_DEVICES, false).toBool()) {
        main->settings()->setValue(Main::OUTPUT_INDEX0, _defaultOutput ? _defaultOutput->indexes()->deviceIndex : -1);
        _defaultOutput->indexes()->outputDisplayIndex = 0;
        addActiveOutput(_defaultOutput);
    }
    if (_activeInputs.size() == 0 && _defaultInput && !main->settings()->value(Main::EXPLICIT_NO_INPUT_DEVICES, false).toBool()) {
        main->settings()->setValue(Main::INPUT_INDEX0, _defaultInput ? _defaultInput->indexes()->deviceIndex : -1);
        _defaultInput->indexes()->inputDisplayIndex = 0;
        addActiveInput(_defaultInput);
    }
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

void AudioEngine::registerAudio(AudioObject* obj) {
    if (!_audioObjectRegistry.contains(obj)) _audioObjectRegistry.append(obj);
}
void AudioEngine::unregisterAudio(AudioObject* obj) {
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

    // Update with the greatest level
    // TODO move this to AudioObjects
    qreal level = 0;
    for (size_t i = 0; i < frames; ++i) {
        float b = buffer[i];
        if (b < 0) b *= -1;
        if (static_cast<qreal>(b) > level) level = static_cast<qreal>(b);
    }
    emit update(level);
}

int AudioEngine::outputCallback(const void* inputBuffer, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {

    CallbackInfo* info = static_cast<CallbackInfo*>(userData);
    std::shared_ptr<DeviceIndexInfo> indexes = info->device->indexes();

    if (info->device->isInputting()) {
        const float* in = static_cast<const float*>(inputBuffer);
        // qDebug() << in[0];
        CallbackInfo* info = static_cast<CallbackInfo*>(userData);
        info->audio->inputObject()->write(in, framesPerBuffer * CHANNELS);

    }

    if (info->device->isOutputting()) {
        float* out = static_cast<float*>(outputBuffer);
        info->audio->mix(out, framesPerBuffer, CHANNELS, indexes->outputListIndex, info->device->volume(), info->audio->activeOutputs().size() == 1);

        // Mix from the InputObject, if this device is set to output it
        AudioObjectInput* input = info->audio->inputObject();
        if ((input->isActiveOutput0() && indexes->outputDisplayIndex == 0)
                || (input->isActiveOutput1() && indexes->outputDisplayIndex == 1)) {
            bool single = (input->isActiveOutput0() && !input->isActiveOutput1()) || (!input->isActiveOutput0() && input->isActiveOutput1());
            info->audio->inputObject()->mix(out, framesPerBuffer, CHANNELS, indexes->outputListIndex, info->device->volume(), single);
        }
    }
    return paContinue;
}

int AudioEngine::inputCallback(const void* inputBuffer, void* /*outputBuffer*/,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {


}
