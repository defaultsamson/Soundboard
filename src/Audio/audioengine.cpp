#include "audioengine.h"

#include <QAudioDeviceInfo>

AudioEngine::AudioEngine() {
    refreshDevices();
}

QList<QAudioDeviceInfo> AudioEngine::devices() {
    return _devices;
}

void AudioEngine::refreshDevices() {
    _devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
}
