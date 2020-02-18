#include "device.h"

#include <memory>

Device::Device(HostInfoContainer* host,
               const PaDeviceInfo* info,
               size_t channels,
               std::shared_ptr<DeviceIndexInfo> indexes,
               bool isInput) :
    _host(host),
    _info(info),
    _channels(channels),
    _indexes(indexes),
    _isInput(isInput)
{

}

// 0 - 100
void Device::setVolume(int volumeInt) {
    _volumeInt = volumeInt;
    _volume = volumeInt / static_cast<float>(100);
}
