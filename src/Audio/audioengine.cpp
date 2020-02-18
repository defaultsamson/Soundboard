#include "audioengine.h"

#include "audioobjectfile.h"
#include <portaudio.h>
#include "../mainapp.h"

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
size_t AudioEngine::FRAMES_PER_BUFFER = 256;
size_t AudioEngine::CHANNELS = 2;

AudioEngine::AudioEngine(Main* main) : main(main) {
}
AudioEngine::~AudioEngine() {
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
}

DeviceInfoContainer* AudioEngine::defaultOutput() {
    return _defaultOutput;
}
DeviceInfoContainer* AudioEngine::defaultInput() {
    return _defaultInput;
}

void AudioEngine::addActiveDevice(DeviceInfoContainer *device) {
    if (!device) return;

    bool in = device->isInput;
    if (in && _activeInputs.contains(device)) return;
    if (!in && _activeOutputs.contains(device)) return;

    // Adding device to the end of the list
    device->indexes.deviceListIndex = in ? _activeInputs.size() : _activeOutputs.size();

    // Add the device to its respective list
    if (in) _activeInputs.append(device);
    else _activeOutputs.append(device);

    _selectedDeviceIndexes.append(device->indexes);

    Pa_CloseStream(device->stream);
    device->stream = nullptr;
    // size_t channels = CHANNELS <= device->info->maxOutputChannels ? CHANNELS : device->info->maxOutputChannels;

    PaError err;
    PaStreamParameters inputParameters;
    PaStreamParameters outputParameters;
    if (in) {
        inputParameters.device = device->indexes.deviceIndex;
        inputParameters.channelCount = static_cast<int>(CHANNELS); // device.info->maxOutputChannels;
        inputParameters.sampleFormat = paFloat32;
        inputParameters.suggestedLatency = device->info->defaultLowOutputLatency ;
        inputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field
    } else {
        outputParameters.device = device->indexes.deviceIndex;
        outputParameters.channelCount = static_cast<int>(CHANNELS); // device.info->maxOutputChannels;
        outputParameters.sampleFormat = paFloat32;
        outputParameters.suggestedLatency = device->info->defaultLowOutputLatency ;
        outputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field
    }

    err = Pa_OpenStream(
                    &device->stream,
                    in ? &inputParameters : nullptr,
                    !in ? &outputParameters : nullptr,
                    device->info->defaultSampleRate,
                    FRAMES_PER_BUFFER,
                    paNoFlag, //flags that can be used to define dither, clip settings and more
                    in ? &AudioEngine::inputCallback : &AudioEngine::outputCallback, //your callback function
                    static_cast<CallbackInfo*>(new CallbackInfo{this, device})); //data to be passed to callback. In C++, it is frequently (void *)this
    //don't forget to check errors!
    if (err != paNoError) qDebug() << "Error opening stream";
    // else qDebug() << "Stream opened successfully!";

    err = Pa_StartStream(device->stream);
    if (err != paNoError) qDebug() << "Error starting stream";
    // else qDebug() << "Stream started successfully!";
}
void AudioEngine::removeActiveDevice(DeviceInfoContainer* device) {
    Pa_CloseStream(device->stream);
    device->indexes.displayIndex = -1;
    // Remove its info so upon reaload or refresh it doesn't select the device again
    for (int i = 0; i < _selectedDeviceIndexes.size(); i++) {
        if (_selectedDeviceIndexes.at(i).deviceIndex == device->indexes.deviceIndex) {
            _selectedDeviceIndexes.removeAt(i);
            break;
        }
    }
    if (device->isInput) {
        _activeInputs.removeOne(device);
        // Updates the deviceListIndex data
        for (int i = 0; i < _activeInputs.size(); i++) {
            _activeInputs.at(i)->indexes.deviceListIndex = i;
        }
    } else {
        _activeOutputs.removeOne(device);
        // Updates the deviceListIndex data
        for (int i = 0; i < _activeOutputs.size(); i++) {
            _activeOutputs.at(i)->indexes.deviceListIndex = i;
        }
    }
}

void AudioEngine::removeActiveDisplayOutput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeOutputs) if (dev->indexes.displayIndex == deviceDisplayIndex) { removeActiveDevice(dev); break; }
}
DeviceInfoContainer* AudioEngine::getActiveDisplayOutput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeOutputs) if (dev->indexes.displayIndex == deviceDisplayIndex) return dev;
    return nullptr;
}
const QList<DeviceInfoContainer*> AudioEngine::activeOutputs() {
    return _activeOutputs;
}

void AudioEngine::removeActiveDisplayInput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeInputs) if (dev->indexes.displayIndex == deviceDisplayIndex) { removeActiveDevice(dev); break; }
}
DeviceInfoContainer* AudioEngine::getActiveDisplayInput(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeInputs) if (dev->indexes.displayIndex == deviceDisplayIndex) return dev;
    return nullptr;
}
const QList<DeviceInfoContainer*> AudioEngine::activeInputs() {
    return _activeInputs;
}

const QList<HostInfoContainer*> AudioEngine::hosts() {
    return _hosts;
}
const QList<DeviceInfoContainer*> AudioEngine::outputs() {
    return _outputs;
}
const QList<DeviceInfoContainer*> AudioEngine::inputs() {
    return _inputs;
}

void AudioEngine::init() {
    Pa_Initialize();
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
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
    _hosts.clear();
    _outputs.clear();
    _inputs.clear();
    for (auto dev : _activeOutputs) Pa_CloseStream(dev->stream);
    _activeOutputs.clear();

    const PaDeviceInfo* device;

    for (int i = 0; i < devices; ++i ) {
        device = Pa_GetDeviceInfo(i);
        bool isInput = device->maxOutputChannels == 0;
        DeviceInfoContainer* dev = dev = new DeviceInfoContainer{nullptr, device, nullptr, CHANNELS, DeviceIndexInfo{i, -1, -1}, 100, 1, isInput};

        if (isInput) {
            _inputs.append(dev);
            if (Pa_GetDefaultInputDevice() == i) {
                _defaultInput = dev;
                qDebug() << "Default input device... [" << i << "]";
            }
        } else {
            _outputs.append(dev);
            if (Pa_GetDefaultOutputDevice() == i) {
                _defaultOutput = dev;
                qDebug() << "Default output device... [" << i << "]";
            }
        }

        // Try to find the container for the specific host that contains all its devices
        bool foundCon = false;
        for (auto hostCon : _hosts) {
            if (hostCon->index == device->hostApi) {
                foundCon = true;
                dev->host = hostCon;
                hostCon->devices->append(dev);
                break;
            }
        }
        // Create the container if not found
        if (!foundCon) {
            QList<DeviceInfoContainer*> *list = new QList<DeviceInfoContainer*>();
            list->append(dev);
            HostInfoContainer* hostCon = new HostInfoContainer{
                device->hostApi,
                Pa_GetHostApiInfo(device->hostApi)->name,
                list
            };
            dev->host = hostCon;
            _hosts.append(hostCon);
        }
    }

    // Devices are loaded, now determine which are supposed to be active
    DeviceInfoContainer* out0 = getDevice(main->settings()->value(Main::OUTPUT_INDEX0, -1).toInt());
    DeviceInfoContainer* out1 = getDevice(main->settings()->value(Main::OUTPUT_INDEX1, -1).toInt());
    DeviceInfoContainer* in0 = getDevice(main->settings()->value(Main::INPUT_INDEX0, -1).toInt());

    // There's a saved device, load it
    if (out0) {
        out0->indexes.displayIndex = 0;
        out0->volumeInt = main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt();
        out0->volume = main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt() / static_cast<float>(100);
        addActiveDevice(out0);
    }
    if (out1) {
        out1->indexes.displayIndex = 1;
        out1->volumeInt = main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt();
        out1->volume = main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt() / static_cast<float>(100);
        addActiveDevice(out1);
    }
    if (in0) {
        in0->indexes.displayIndex = 0;
        in0->volumeInt = main->settings()->value(Main::INPUT_VOLUME0, 100).toInt();
        in0->volume = main->settings()->value(Main::INPUT_VOLUME0, 100).toInt() / static_cast<float>(100);
        addActiveDevice(in0);
    }

    // If no device was found, and the devices weren't explicitly all removed, load the defaults
    if (_activeOutputs.size() == 0 && _defaultOutput && !main->settings()->value(Main::EXPLICIT_NO_OUTPUT_DEVICES, false).toBool()) {
        main->settings()->setValue(Main::OUTPUT_INDEX0, _defaultOutput ? _defaultOutput->indexes.deviceIndex : -1);
        _defaultOutput->indexes.displayIndex = 0;
        addActiveDevice(_defaultOutput);
    }
    if (_activeInputs.size() == 0 && _defaultInput && !main->settings()->value(Main::EXPLICIT_NO_INPUT_DEVICES, false).toBool()) {
        main->settings()->setValue(Main::INPUT_INDEX0, _defaultInput ? _defaultInput->indexes.deviceIndex : -1);
        _defaultInput->indexes.displayIndex = 0;
        addActiveDevice(_defaultInput);
    }
}

DeviceInfoContainer* AudioEngine::getDevice(int deviceIndex) {
    for (auto dev : _outputs)
        if (dev->indexes.deviceIndex == deviceIndex)
            return dev;

    for (auto dev : _inputs)
        if (dev->indexes.deviceIndex == deviceIndex)
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

    for (AudioObjectFile* audio : _audioObjectRegistry) {
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

int AudioEngine::outputCallback(const void* /*inputBuffer*/, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {

    float *out = static_cast<float*>(outputBuffer);
    CallbackInfo *info = static_cast<CallbackInfo*>(userData);
    info->audio->mix(out, framesPerBuffer, CHANNELS, info->device->indexes.deviceListIndex, info->device->volume, info->audio->activeOutputs().size() == 1);
    return paContinue;
}

int AudioEngine::inputCallback(const void* inputBuffer, void* /*outputBuffer*/,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {

    const float *in = static_cast<const float*>(inputBuffer);
    CallbackInfo *info = static_cast<CallbackInfo*>(userData);
    //info->audio->mix(in, framesPerBuffer, CHANNELS, info->device->indexes.deviceListIndex, info->device->volume, info->audio->activeOutputs().size() == 1);
    return paContinue;
}
