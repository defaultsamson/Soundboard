#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>

AudioEngine::AudioEngine() : _devices(), _activeDevices() {
    refreshDevices();
}

QList<DeviceInfoContainer> AudioEngine::devices() {
    return _devices;
}

QList<DeviceInfoContainer> AudioEngine::activeDevices() {
    return _activeDevices;
}

void AudioEngine::refreshDevices() {
    // Make the default device active
    // TODO allow changing which devices are active via the settings menu

    qDebug() << "Starting PA audio...";

    const PaDeviceInfo *device;

    Pa_Initialize();

    qDebug() << "Device Count: " << Pa_GetDeviceCount();

    for (int i = 0; i < Pa_GetDeviceCount(); ++i ) {
        device = Pa_GetDeviceInfo(i);
        qDebug() << "------------------------------------------";
        qDebug() << "Device: [" << Pa_GetHostApiInfo(device->hostApi)->name << "] " << device->name;

        if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paALSA)) {
            qDebug() << "  (Backend) ALSA";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paJACK)) {
            qDebug() << "  (Backend) JACK";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paOSS)) {
            qDebug() << "  (Backend) OSS";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paWASAPI)) {
            qDebug() << "  (Backend) WASAPI";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paWDMKS)) {
            qDebug() << "  (Backend) WDMKS";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paCoreAudio)) {
            qDebug() << "  (Backend) CoreAudio";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paASIO)) {
            qDebug() << "  (Backend) ASIO";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paMME)) {
            qDebug() << "  (Backend) MME";
        } else if (device->hostApi == Pa_HostApiTypeIdToHostApiIndex(paDirectSound)) {
            qDebug() << "  (Backend) DirectSound";
        }

        if (device->maxOutputChannels == 0) { // isInput
            qDebug() << "  (Input) " << device->maxInputChannels << " Channels";
        } else {
            qDebug() << "  (Output) " << device->maxOutputChannels << " Channels";
            _devices.append({device, i});
        }
    }
}
