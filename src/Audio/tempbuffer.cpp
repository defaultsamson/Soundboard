#include "tempbuffer.h"

#include <string.h>

TempBuffer::TempBuffer() {
    // Set up _buffer to hold as much data as a device's total requested frames (frames per buffer * channels), times the BUFFER_MULTIPLIER
    _buffer = new float[BUFFER_MULTIPLIER * FRAMES];

    // Set up size_t's that represent how much data is in each section of _buffer
    _readCount = new size_t[BUFFER_MULTIPLIER];
}

TempBuffer::~TempBuffer() {
    delete [] _buffer;
    delete [] _readCount;
}

void TempBuffer::write(const float* buffer, size_t n, float volume, bool overwrite) {
    float* start = _buffer + (FRAMES * _writeIndex);

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy
            memcpy(start, buffer, n * sizeof(float));
        } else {
            for (size_t i = 0; i < n; ++i) start[i] = buffer[i] * volume;
        }
    } else {
        if (volume == 1.0F) {
            for (size_t i = 0; i < n; ++i) start[i] += buffer[i];
        } else {
            for (size_t i = 0; i < n; ++i) start[i] += buffer[i] * volume;
        }
    }

    _readCount[_writeIndex] = n;
    _writeIndex++;

    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    if (_writeIndex >= BUFFER_MULTIPLIER) {
        _writingLoopsAhead++;
        _writeIndex = 0;
    }
}

size_t TempBuffer::read(float* buffer, size_t /*n*/, float volume, bool overwrite) {
    // TODO continue filling the buffer if the availableRead < n

    // Ensures that the writing is always ahead of the reading
    if (_writeIndex + (BUFFER_MULTIPLIER * _writingLoopsAhead) > _readIndex) return 0;

    float* start = _buffer + (FRAMES * _writeIndex);

    size_t availableRead = _readCount[_readIndex];
    _readIndex++;

    // If the _readIndex has reached the BUFFER_MULTIPLIER, the next read will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    if (_readIndex >= BUFFER_MULTIPLIER) {
        _readIndex = 0;
        _writingLoopsAhead--;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy
            memcpy(buffer, start, availableRead * sizeof(float));
        } else {
            for (size_t i = 0; i < availableRead; ++i) buffer[i] = start[i] * volume;
        }
    } else {
        if (volume == 1.0F) {
            for (size_t i = 0; i < availableRead; ++i) buffer[i] += start[i];
        } else {
            for (size_t i = 0; i < availableRead; ++i) buffer[i] += start[i] * volume;
        }
    }

    return availableRead;
}

bool TempBuffer::readCaughtUp() {
    // If the _readIndex has caught up fully with _writeIndex
    // Note: _readIndex should never be > _writeIndex, we want this to stop
    // when they are equal, but JUST in case, we use >=
    return _writingLoopsAhead == 0 && _readIndex >= _writeIndex;
}

void TempBuffer::clear() {
    _writeIndex = 0;
    _readIndex = 0;
    _writingLoopsAhead = 0;
    memset(_readCount, 0, BUFFER_MULTIPLIER);
}
