#include "audioobject.h"

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>
#include "audioengine.h"

AudioObject::AudioObject()
{

}
AudioObject::~AudioObject() {
    delete [] sideBuffer;
    delete [] bytesRead;
    delete file;
}

bool AudioObject::isPlaying() { return !isPaused() && !isStopped(); }
bool AudioObject::isPaused() { return paused; }
bool AudioObject::isStopped() { return stopped; }

void AudioObject::stop() {
    paused = false;
    stopped = true;
    device0Finished = false;
    if (file) {
        // We can never seek whilst reading
        safeRead.lock();
        file->seek(0, SEEK_SET);
        safeRead.unlock();
    }
}

void AudioObject::pause() {
    if (!file || stopped) return;
    paused = true;
    stopped = false;
}

void AudioObject::play() {
    if (!file) return;
    if (paused) {
        // Resume
        paused = false;
    } else if (stopped) {
        stopped = false;
    }
}

void AudioObject::mix(float* buffer, size_t /*framesPerBuffer*/, size_t /*channels*/, int deviceListIndex, float deviceVolume, bool singleDevice) {
    if (!file) return;
    // If it's paused or stopped, return
    if (paused || stopped) return;
    // The 2nd device is still reading, if this is being called by the first device, return
    if (device0Finished && deviceListIndex == 0) return;

    // The frames to read from the file. This is hardcoded for now because of how sideBuffer is initialized
    size_t frames = AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;
    // The volume of the device and this audio object combined
    float finalVolume = _volume * deviceVolume;

    // If it's the first device, then load up the sideBuffer
    if (deviceListIndex == 0) {
        float *readBuffer = new float[frames];

        // Read frames from the file to the readBuffer
        safeRead.lock();
        size_t read = static_cast<size_t>(file->read(readBuffer, static_cast<sf_count_t>(frames)));
        safeRead.unlock();

        if (!singleDevice) {
            // Copy the readBuffer into sideBuffer
            size_t start = frames * sideBufferWrite;
            memcpy(sideBuffer + start, readBuffer, read * sizeof(float));
            bytesRead[sideBufferWrite] = read;
        }

        // Read the amount of bytes read from mixBuffer into buffer, and apply the volume
        for (size_t i = 0; i < read; i++){
            buffer[i] += readBuffer[i] * finalVolume;
        }

        delete [] readBuffer;

        if (!singleDevice) {
            sideBufferWrite++;
            // If the sideBufferWrite has reached the SIDE_BUFFER_MULTIPLIER, the next write will
            // surpass the limit of sideBuffer. To avoid this, we loop it back around to the beginning
            if (sideBufferWrite >= SIDE_BUFFER_MULTIPLIER) {
                device0LoopsAhead++;
                sideBufferWrite = 0;
            }
        }

        // Ran out of bytes to read, the file stream is over
        if (read == 0 || read < frames) {
            if (singleDevice) stop();
            else device0Finished = true;
        }

    // Makes sure that the sideBuffer writing is always ahead of the reading
    } else if (sideBufferWrite + (SIDE_BUFFER_MULTIPLIER * device0LoopsAhead) > sideBufferRead) {
        size_t start = frames * sideBufferRead;
        // Read the amount of bytes read from sideBuffer into buffer, and apply the volume
        for (size_t i = 0; i < bytesRead[sideBufferRead]; i++){
            buffer[i] += sideBuffer[start + i] * finalVolume;
        }

        sideBufferRead++;
        // If the sideBufferRead has reached the SIDE_BUFFER_MULTIPLIER, the next read will
        // surpass the limit of sideBuffer. To avoid this, we loop it back around to the beginning
        if (sideBufferRead >= SIDE_BUFFER_MULTIPLIER) {
            sideBufferRead = 0;
            device0LoopsAhead--;
        }
        // If the first device stopped reading the file, and the other device has caught up
        // Note: sideBufferRead should never be > sideBufferWrite, we want this to stop
        // when they are equal, but JUST in case, we use >=
        if (device0Finished && device0LoopsAhead == 0 && sideBufferRead >= sideBufferWrite) {
            stop();
        }
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    if (file) delete file;

    // Set up the sideBuffer to hold as much data as a device's total requested frames (frames per buffer * channels), times the SIDE_BUFFER_MULTIPLIER
    sideBuffer = new float[SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS];
    memset(sideBuffer, 0, SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float));

    // Set up a size_t for each buffer in sideBuffer, so we know how much data is in each section of sideBuffer
    bytesRead = new size_t[SIDE_BUFFER_MULTIPLIER];
    memset(bytesRead, 0, SIDE_BUFFER_MULTIPLIER * sizeof(size_t));

    sideBufferWrite = 0;
    sideBufferRead = 0;

    file = new SndfileHandle(filename.toStdString());
}

bool AudioObject::hasFile() {
    return file;
}

void AudioObject::setVolume(const float volume) {
    _volume = volume;
}
