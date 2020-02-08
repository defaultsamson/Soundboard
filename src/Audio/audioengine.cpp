#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>
#include "../mainapp.h"

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER 256
#define CHANNELS 2

AudioEngine::AudioEngine(Main *main) : main(main) {
}
AudioEngine::~AudioEngine() {
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
}

HostInfoContainer *AudioEngine::defaultHost() {
    return _defaultHost;
}
DeviceInfoContainer *AudioEngine::defaultDevice() {
    return _defaultDevice;
}

void AudioEngine::addActiveDevice(DeviceInfoContainer *device) {
    if (_activeDevices.contains(device) || !device) return;
    _activeDevices.append(device);
    _selectedDeviceIndexes.append(device->indexes);

    Pa_CloseStream(device->stream);
    device->stream = nullptr;
    int channels = CHANNELS <= device->info->maxOutputChannels ? CHANNELS : device->info->maxOutputChannels;

    PaError err;
    PaStreamParameters outputParameters;
    // bzero( &outputParameters, sizeof( outputParameters ) ); // not necessary if you are filling in all the fields
    outputParameters.device = device->indexes.deviceIndex;
    outputParameters.channelCount = channels; // device.info->maxOutputChannels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = device->info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field

    err = Pa_OpenStream(
                    &device->stream,
                    nullptr,
                    &outputParameters,
                    device->info->defaultSampleRate,
                    FRAMES_PER_BUFFER,
                    paNoFlag, //flags that can be used to define dither, clip settings and more
                    &AudioEngine::readCallback, //your callback function
                    (void *) this); //data to be passed to callback. In C++, it is frequently (void *)this
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
    // Remove its info so upon reaload or refresh it doesn't select it again
    for (int i = 0; i < _selectedDeviceIndexes.size(); i++)
        if (_selectedDeviceIndexes.at(i).deviceIndex == device->indexes.deviceIndex)
            _selectedDeviceIndexes.removeAt(i);
    _activeDevices.removeOne(device);
}
void AudioEngine::removeActiveDevice(int deviceDisplayIndex) { // makes controlling easier from the settings dialogue
    for (auto dev : _activeDevices) if (dev->indexes.displayIndex == deviceDisplayIndex) removeActiveDevice(dev);
}
const QList<DeviceInfoContainer*> AudioEngine::activeDevices() {
    return _activeDevices;
}

const QList<HostInfoContainer*> AudioEngine::hosts() {
    return _hosts;
}
const QList<DeviceInfoContainer*> AudioEngine::devices() {
    return _devices;
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
    // Make the default device active
    // TODO allow changing which devices are active via the settings menu

    int devices = Pa_GetDeviceCount();
    qDebug() << "Refreshing devices... [" << devices << "]";

    _defaultHost = nullptr;
    _defaultDevice = nullptr;
    for (int i = 0; i < _hosts.size(); i++) delete _hosts.at(i);
    _hosts.clear();
    _devices.clear();
    _activeDevices.clear();

    const PaDeviceInfo *device;

    for (int i = 0; i < devices; ++i ) {
        device = Pa_GetDeviceInfo(i);
        if (device->maxOutputChannels == 0) { // isInput
            qDebug() << "Ignoring input channel... [" << i << "]";
        } else {
            DeviceInfoContainer *dev = new DeviceInfoContainer{nullptr, device, nullptr, CHANNELS, DeviceIndexInfo{i, -1}};
            _devices.append(dev);
            if (Pa_GetDefaultOutputDevice() == i) {
                _defaultDevice = dev;
                qDebug() << "Default device... [" << i << "]";
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
                HostInfoContainer *hostCon = new HostInfoContainer{
                    device->hostApi,
                    Pa_GetHostApiInfo(device->hostApi)->name,
                    list
                };
                dev->host = hostCon;
                _hosts.append(hostCon);
                // Default stuff
                if (device->hostApi == Pa_GetDefaultHostApi()) {
                    _defaultHost = hostCon;
                    qDebug() << "Default host... [" << device->hostApi << "]";
                }
            }
        }
    }

    // Devices are loaded, now determine which are supposed to be active
    DeviceInfoContainer* dev0 = getDevice(main->settings()->value(Main::DEVICE_INDEX0, -1).toInt());
    DeviceInfoContainer* dev1 = getDevice(main->settings()->value(Main::DEVICE_INDEX1, -1).toInt());

    // There's a saved device, load it
    if (dev0) {
        dev0->indexes.displayIndex = 0;
        addActiveDevice(dev0);
    }
    if (dev1) {
        dev1->indexes.displayIndex = 1;
        addActiveDevice(dev1);
    }

    // If no device was found, load the defaults
    if (_activeDevices.size() == 0 && _defaultDevice && !main->settings()->value(Main::EXPLICIT_NO_DEVICES, false).toBool()) {
        main->settings()->setValue(Main::DEVICE_INDEX0, _defaultDevice ? _defaultDevice->indexes.deviceIndex : -1);
        _defaultDevice->indexes.displayIndex = 0;
        addActiveDevice(_defaultDevice);
    }
}

DeviceInfoContainer* AudioEngine::getDevice(int deviceIndex) {
    for (auto dev : _devices)
        if (dev->indexes.deviceIndex == deviceIndex)
            return dev;

    return nullptr;
}

void AudioEngine::registerAudio(AudioObject *obj) {
    if (!_audioObjectRegistry.contains(obj)) _audioObjectRegistry.append(obj);
}
void AudioEngine::unregisterAudio(AudioObject *obj) {
    if (_audioObjectRegistry.contains(obj)) _audioObjectRegistry.removeOne(obj);
}

// Mixes audio from all the AudioObjects (future: perhaps mic too?)
void AudioEngine::mix(float* buffer, size_t framesPerBuffer) {

    size_t frames = framesPerBuffer * CHANNELS;

    // Fills the buffer with zeros
    memset(buffer, 0, frames * sizeof(float));

    for (AudioObject *audio : _audioObjectRegistry) {
        audio->mix(buffer, framesPerBuffer);
    }

    // Update with the greatest level
    qreal level = 0;
    for (size_t i = 0; i < frames; ++i) {
        float b = buffer[i];
        if (b < 0) b *= -1;
        if (b  > level) level = b;
    }
    emit update(level);
}

int AudioEngine::readCallback(const void* /*inputBuffer*/, void *outputBuffer,
                              unsigned long framesPerBuffer,
                              const PaStreamCallbackTimeInfo* /*timeInfo*/,
                              PaStreamCallbackFlags /*statusFlags*/,
                              void *userData) {

    float *out = static_cast<float*>(outputBuffer);
    AudioEngine *audio = static_cast<AudioEngine*>(userData);
    audio->mix(out, framesPerBuffer);
    return paContinue;

    /*
     * To ensure that the callback continues
 to be called, it should return paContinue (0). Either paComplete or paAbort
 can be returned to finish stream processing, after either of these values is
 returned the callback will not be called again. If paAbort is returned the
 stream will finish as soon as possible. If paComplete is returned, the stream
 will continue until all buffers generated by the callback have been played.
 This may be useful in applications such as soundfile players where a specific
 duration of output is required. However, it is not necessary to utilize this
 mechanism as Pa_StopStream(), Pa_AbortStream() or Pa_CloseStream() can also
 be used to stop the stream. The callback must always fill the entire output
 buffer irrespective of its return value.
 */
}
