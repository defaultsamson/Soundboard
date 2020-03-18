#include "audioobjectinput.h"

void AudioObjectInput::stop() {
    if (stopped) return;
    AudioObject::stop();
    _inBuffer.clear();
}

void AudioObjectInput::write(const float* buffer, size_t n) {
    // If the device is stopped, still update the visualizers
    if (stopped) {
        float maxLevel = 0;
        // Read the amount of bytes read from mixBuffer into buffer, and apply the volume
        for (size_t i = 0; i < n; i++){
            float final = buffer[i];
            // Update with the greatest level
            if (final < 0) final *= -1;
            if (final > maxLevel) maxLevel = final;
        }
        emit update(maxLevel);
    }
    if ((!(_output0 && _hasOutput0) && !(_output1 && _hasOutput1)) || !_hasInputDevice) {
        if (!stopped) stop();
    } else {
        stopped = false;
        _inBuffer.write(buffer, n);
    }
}

size_t AudioObjectInput::read(float* buffer, size_t n) {
    if (stopped) return 0;
    if ((!_output0 && !_output1) || !_hasInputDevice) {
        stop();
        return 0;
    }
    // Makes sure that the inBuffer writing is always ahead of the reading
    if (_inBuffer.writingAhead()) {
        size_t read = _inBuffer.read(buffer, n);

        if (read < n) stop();

        return read;
    }
    return 0;
}
