#include "audioengine.h"

#include <QAudioDeviceInfo>

AudioEngine::AudioEngine() : _activeDevices() {
    refreshDevices();
}

QList<QAudioDeviceInfo> AudioEngine::devices() {
    return _devices;
}

QList<QAudioDeviceInfo> AudioEngine::activeDevices() {
    return _activeDevices;
}

void AudioEngine::refreshDevices() {
    _devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
    _activeDevices.append(QAudioDeviceInfo::defaultOutputDevice()); // Make the default device active
    // TODO allow changing which devices are active via the settings menu
}
