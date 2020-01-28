#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER 256
#define CHANNELS 2

AudioEngine::AudioEngine() {
}

bool AudioEngine::hasDefaultHost() {
    return _hasDefaultHost;
}
HostInfoContainer AudioEngine::defaultHost() {
    return _defaultHost;
}
bool AudioEngine::hasDefaultDevice() {
    return _hasDefaultDevice;
}
DeviceInfoContainer AudioEngine::defaultDevice() {
    return _defaultDevice;
}
bool AudioEngine::hasSelectedHost() {
    return _hasSelectedHost;
}
void AudioEngine::setSelectedHost(HostInfoContainer host) {
    _selectedHost = host;
    _hasSelectedHost = true;
}
HostInfoContainer AudioEngine::selectedHost() {
    return _selectedHost;
}
bool AudioEngine::hasSelectedDevice() {
    return _hasSelectedDevice;
}
void AudioEngine::setSelectedDevice(DeviceInfoContainer device) {
    _selectedDevice = device;
    _hasSelectedDevice = true;
    selectedDeviceIndex = device.index;

    Pa_CloseStream(stream);
    stream = nullptr;
    channels = CHANNELS;

    PaError err;
    PaStreamParameters outputParameters;
    // bzero( &outputParameters, sizeof( outputParameters ) ); // not necessary if you are filling in all the fields
    outputParameters.device = device.index;
    outputParameters.channelCount = channels; // device.info->maxOutputChannels;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = device.info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field

    err = Pa_OpenStream(
                    &stream,
                    nullptr,
                    &outputParameters,
                    device.info->defaultSampleRate,
                    FRAMES_PER_BUFFER,
                    paNoFlag, //flags that can be used to define dither, clip settings and more
                    &AudioEngine::readCallback, //your callback function
                    (void *) this); //data to be passed to callback. In C++, it is frequently (void *)this
    //don't forget to check errors!
    if (err != paNoError) qDebug() << "Error opening stream";
    else qDebug() << "Stream opened successfully!";

    err = Pa_StartStream(stream);
    if (err != paNoError) qDebug() << "Error starting stream";
    else qDebug() << "Stream started successfully!";
}
DeviceInfoContainer AudioEngine::selectedDevice() {
    return _selectedDevice;
}

QList<HostInfoContainer> AudioEngine::hosts() {
    return _hosts;
}

bool AudioEngine::hasActiveHost() {
    return hasSelectedHost() || hasDefaultHost();
}

HostInfoContainer AudioEngine::activeHost() {
    return hasSelectedHost() ? selectedHost() : defaultHost();
}

bool AudioEngine::hasActiveDevice() {
    return hasSelectedDevice() || hasDefaultDevice();
}

DeviceInfoContainer AudioEngine::activeDevice() {
    return hasSelectedDevice() ? selectedDevice() : defaultDevice();
}

void AudioEngine::init() {
    qDebug() << "Starting PortAudio...";
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

    _hasDefaultHost = false;
    _hasDefaultDevice = false;
    _hosts.clear();

    const PaDeviceInfo *device;

    for (int i = 0; i < devices; ++i ) {
        device = Pa_GetDeviceInfo(i);
        if (device->maxOutputChannels == 0) { // isInput
            qDebug() << "Ignoring input channel... [" << i << "]";
        } else {
            DeviceInfoContainer dev = {device, i};
            if (Pa_GetDefaultOutputDevice() == i) {
                _defaultDevice = dev;
                _hasDefaultDevice = true;
                qDebug() << "Default device... [" << i << "]";

                // unspecified selected device, select default device
                if (selectedDeviceIndex < 0) selectedDeviceIndex = i;
            }
            // If it's the device to select
            if (i == selectedDeviceIndex) setSelectedDevice(dev);
            // Try to find the container for the specific host that contains all its devices
            bool foundCon = false;
            for (auto hostCon : _hosts) {
                if (hostCon.hostIndex == device->hostApi) {
                    foundCon = true;
                    hostCon.devices->append(dev);
                    // If it's the device to select, set the selected host to the one it belongs to
                    if (i == selectedDeviceIndex) setSelectedHost(hostCon);
                    break;
                }
            }
            // Create the container if not found
            if (!foundCon) {
                QList<DeviceInfoContainer> *list = new QList<DeviceInfoContainer>();
                list->append(dev);
                HostInfoContainer hostCon = {
                    device->hostApi,
                    Pa_GetHostApiInfo(device->hostApi)->name,
                    list
                };
                _hosts.append(hostCon);
                // Default stuff
                if (device->hostApi == Pa_GetDefaultHostApi()) {
                    _defaultHost = hostCon;
                    _hasDefaultHost = true;
                    qDebug() << "Default host... [" << device->hostApi << "]";

                    // If it's the device to select, set the selected host to the one it belongs to
                    if (i == selectedDeviceIndex) setSelectedHost(hostCon);
                }
            }
        }
    }
}

void AudioEngine::registerAudio(AudioObject *obj) {
    if (!_audioObjectRegistry.contains(obj)) _audioObjectRegistry.append(obj);
}
void AudioEngine::unregisterAudio(AudioObject *obj) {
    if (_audioObjectRegistry.contains(obj)) _audioObjectRegistry.removeOne(obj);
}

// Mixes audio from all the AudioObjects (future: perhaps mic too?)
void AudioEngine::mix(float* buffer, size_t framesPerBuffer) {

    size_t frames = framesPerBuffer * channels;

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

    /*for (size_t i = 0; i < framesPerBuffer; ++i) {
        *out++ = 0; // Left
        *out++ = 0; // Right
    }*/
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

    /*
    const void *data;
    assert(s);
    assert(length > 0);

    if (stdio_event)
        mainloop_api->io_enable(stdio_event, PA_IO_EVENT_OUTPUT);

    if (pa_stream_peek(s, &data, &length) < 0) {
        fprintf(stderr, "pa_stream_peek() failed: %s\n", pa_strerror(pa_context_errno(context)));
        quit(1);
        return;
    }

    assert(data);
    assert(length > 0);

    if (buffer) {
        buffer = pa_xrealloc(buffer, buffer_length + length);
        memcpy((uint8_t*) buffer + buffer_length, data, length);
        buffer_length += length;
    } else {
        buffer = pa_xmalloc(length);
        memcpy(buffer, data, length);
        buffer_length = length;
        buffer_index = 0;
    }

    pa_stream_drop(s);
    */
}
