#include "audioobject.h"

AudioObject::AudioObject() {
}

AudioObject::~AudioObject() {
}

void AudioObject::stop() {
    if (stopped) return;
    emit update(0);
    stopped = true;
    device0Finished = false;
    _tempBuffer.clear();
}

void AudioObject::mix(float* buffer, size_t /*framesPerBuffer*/, size_t /*channels*/, int deviceListIndex, float deviceVolume, bool singleDevice) {
    if (stopped) return;
    if (!doMix()) return;
    // The 2nd device is still reading, if this is being called by the first device, return
    if (device0Finished && deviceListIndex == 0) return;

    // The frames to read from the file. This is hardcoded for now because of how sideBuffer is initialized
    size_t frames = 256 * 2;
    // The volume of the device and this audio object combined
    float finalVolume = _volume * deviceVolume;

    // If it's the first device (or the only device), then read from this
    if (deviceListIndex == 0 || singleDevice) {
        float readBuffer[frames];

        // Read frames from this to the readBuffer
        size_t readCount = static_cast<size_t>(this->read(readBuffer, frames));
        if (readCount == 0) {
            emit update(0);
            return;
        }

        // If it's not the only device, sill the tempBuffer
        if (!singleDevice)
            _tempBuffer.write(readBuffer, readCount);

        float maxLevel = 0;
        // Read the amount of bytes read from mixBuffer into buffer, and apply the volume
        for (size_t i = 0; i < readCount; i++){
            // Update with the greatest level
            if (_updateVisualiser) {
                float final = readBuffer[i] * finalVolume;
                buffer[i] += final;

                if (final < 0) final *= -1;
                if (final > maxLevel) maxLevel = final;
            } else {
                buffer[i] += readBuffer[i] * finalVolume;
            }
        }
        emit update(maxLevel);

        // Ran out of bytes to read, the file stream is over
        if (readCount == 0 || readCount < frames) {
            if (singleDevice) stop();
            else device0Finished = true;
        }

    // Makes sure that the sideBuffer writing is always ahead of the reading
    } else {
        _tempBuffer.read(buffer, frames, finalVolume, false);

        // If the first device stopped reading the file, and the other device has caught up
        // Note: sideBufferRead should never be > sideBufferWrite, we want this to stop
        // when they are equal, but JUST in case, we use >=
        if (device0Finished && _tempBuffer.readCaughtUp()) {
            stop();
        }
    }
}
