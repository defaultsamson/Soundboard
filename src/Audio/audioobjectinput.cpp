#include "audioobjectinput.h"

#include "audioengine.h"

#include <iostream>

AudioObjectInput::AudioObjectInput() {
    // Set up the inBuffer to hold as much data as a device's total requested frames (frames per buffer * channels), times the SIDE_BUFFER_MULTIPLIER
    inBuffer = new float[SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS];
    // unneccessary? memset(inBuffer, 0, SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float));

    // Set up a size_t for each buffer in inBuffer that represents how much data is in each section of inBuffer
    bytesWritten = new size_t[SIDE_BUFFER_MULTIPLIER];
    // unneccessary? memset(bytesWritten, 0, SIDE_BUFFER_MULTIPLIER * sizeof(size_t));
}
AudioObjectInput::~AudioObjectInput() {
    delete [] inBuffer;
    delete [] bytesWritten;
}

void AudioObjectInput::stop() {
    if (stopped) return;
    AudioObject::stop();
    inBufferWrite = 0;
    inBufferRead = 0;
    inBufferLoopsAhead = 0;
}

void AudioObjectInput::write(const float* buffer, size_t n) {
    if ((!_output0 && !_output1) || !_hasInputDevice) {
        stop();
        return;
    }
    stopped = false;

    bytesWritten[inBufferWrite] = n;

    size_t start = n * inBufferWrite;

    memcpy(inBuffer + start, buffer, n * sizeof(float));

    inBufferWrite++;
    // If the inBufferWrite has reached the SIDE_BUFFER_MULTIPLIER, the next write will
    // surpass the limit of inBuffer. To avoid this, we loop it back around to the beginning
    if (inBufferWrite >= SIDE_BUFFER_MULTIPLIER) {
        inBufferLoopsAhead++;
        inBufferWrite = 0;
    }
}

size_t AudioObjectInput::read(float* buffer, size_t n) {
    if ((!_output0 && !_output1) || !_hasInputDevice) {
        stop();
        return 0;
    }
    if (stopped) return 0;
    // Makes sure that the inBuffer writing is always ahead of the reading
    if (inBufferWrite + (SIDE_BUFFER_MULTIPLIER * inBufferLoopsAhead) > inBufferRead) {
        // The frames to read from the file. This is hardcoded for now because of how sideBuffer is initialized
        //size_t frames = AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;
        size_t start = n * inBufferRead;

        size_t toRead = bytesWritten[inBufferRead];
        memcpy(buffer, inBuffer + start, toRead * sizeof(float));

        inBufferRead++;
        // If the inBufferRead has reached the SIDE_BUFFER_MULTIPLIER, the next read will
        // surpass the limit of inBuffer. To avoid this, we loop it back around to the beginning
        if (inBufferRead >= SIDE_BUFFER_MULTIPLIER) {
            inBufferRead = 0;
            inBufferLoopsAhead--;
        }

        if (toRead < n) stop();

        return toRead;
    }
    else return 0;
}
