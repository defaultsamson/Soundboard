#include "device.h"

Device::Device(HostInfoContainer* host,
               const PaDeviceInfo* info,
               size_t channels,
               std::shared_ptr<DeviceIndexInfo> indexes,
               bool isInput,
               bool isOutput) :
    _host(host),
    _info(info),
    _channels(channels),
    _indexes(indexes),
    _isInput(isInput),
    _isOutput(isOutput)
{

}

// 0 - 100
void Device::setVolumeInt(int volumeInt) {
    _volumeInt = volumeInt;
    _volume = volumeInt / static_cast<float>(100);
}
