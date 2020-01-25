#include "audioengine.h"

#include "audioobject.h"
#include <portaudio.h>

AudioEngine::AudioEngine() {
    qDebug() << "Starting PortAudio...";
    Pa_Initialize();
    refreshDevices();
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

void AudioEngine::refreshDevices() {
    // Make the default device active
    // TODO allow changing which devices are active via the settings menu

    int devices = Pa_GetDeviceCount();
    qDebug() << "Refreshing devices... [" << devices << "]";

    _hasDefaultHost = false;
    _hasDefaultDevice = false;

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
            }
            // Try to find the container for the specific host that contains all its devices
            bool foundCon = false;
            for (auto hostCon : _hosts) {
                if (hostCon.hostIndex == device->hostApi) {
                    foundCon = true;
                    hostCon.devices->append(dev);
                    break;
                }
            }
            // Create the container if not found
            if (!foundCon) {
                QList<DeviceInfoContainer> *list = new QList<DeviceInfoContainer>();
                list->append(dev);
                HostInfoContainer infoCon = {
                    device->hostApi,
                    Pa_GetHostApiInfo(device->hostApi)->name,
                    list
                };
                _hosts.append(infoCon);
                // Default stuff
                if (device->hostApi == Pa_GetDefaultHostApi()) {
                    _defaultHost = infoCon;
                    _hasDefaultHost = true;
                    qDebug() << "Default host... [" << device->hostApi << "]";
                }
            }
        }
    }
}
