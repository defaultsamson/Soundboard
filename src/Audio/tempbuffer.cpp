#include "tempbuffer.h"

#include <string.h>

TempBuffer::TempBuffer() {
    // Set up _buffer to hold as much data as a device's total requested frames (frames per buffer * channels), times the BUFFER_MULTIPLIER
    _buffer = new float[BUFFER_BYTES];
}

TempBuffer::~TempBuffer() {
    delete [] _buffer;
}

void TempBuffer::write(const float* buffer, size_t n, float volume, bool overwrite) {
    float* start = _buffer + _writeIndex;

    _writeIndex += n;
    bool writingLooped = false;

    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    // WARNING: This assumes that _writeIndex can only loop ahead at max 1 time
    if (_writeIndex >= BUFFER_BYTES) {
        _writingLoopsAhead++;
        _writeIndex -= BUFFER_BYTES;
        writingLooped = true;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy

            if (writingLooped) {
                size_t preLoop = n - _writeIndex; // The number of bytes at the end of _buffer to write before it loops
                memcpy(start  , buffer          , preLoop     * sizeof(float));
                memcpy(_buffer, buffer + preLoop, _writeIndex * sizeof(float));
            } else {
                memcpy(start  , buffer          , n           * sizeof(float));
            }

        } else {

            if (writingLooped) {
                size_t preLoop = n - _writeIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop    ; ++i) start[i]   = buffer[i]             * volume;
                for (size_t i = 0; i < _writeIndex; ++i) _buffer[i] = (buffer + preLoop)[i] * volume;
            } else {
                for (size_t i = 0; i < n          ; ++i) start[i]   = buffer[i]             * volume;
            }

        }
    } else {
        if (volume == 1.0F) {

            if (writingLooped) {
                size_t preLoop = n - _writeIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop    ; ++i) start[i]   += buffer[i]            ;
                for (size_t i = 0; i < _writeIndex; ++i) _buffer[i] += (buffer + preLoop)[i];
            } else {
                for (size_t i = 0; i < n          ; ++i) start[i]   += buffer[i]            ;
            }

        } else {

            if (writingLooped) {
                size_t preLoop = n - _writeIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop    ; ++i) start[i]   += buffer[i]             * volume;
                for (size_t i = 0; i < _writeIndex; ++i) _buffer[i] += (buffer + preLoop)[i] * volume;
            } else {
                for (size_t i = 0; i < n          ; ++i) start[i]   += buffer[i]             * volume;
            }

        }
    }
}

size_t TempBuffer::read(float* buffer, size_t n, float volume, bool overwrite) {
    // TODO return when there's < n bytes to read

    // Ensures that the writing is always ahead of the reading
    if (!writingAhead() || n == 0) return 0;

    float* start = _buffer + _readIndex;

    size_t maxN = 0;
    // Compute the max n that can be read
    if (_writingLoopsAhead > 0) {
        // If the writing is a loop ahead, the available bytes in _buffer to be read is
        // _buffer = [0 ... <-_writeIndex ... _readIndex-> ... (BUFFER_BYTES - 1)]
        maxN = _writeIndex + (BUFFER_BYTES - 1 - _readIndex);
    } else {
        // _buffer = [0 ... _readIndex-> ... <-_writeIndex ... (BUFFER_BYTES - 1)]
        maxN = _writeIndex - _readIndex;
    }
    if (maxN == 0) return 0;
    n = maxN > n ? n : maxN;

    // Then process the read
    _readIndex += n;
    bool readingLooped = false;

    // If the _readIndex has reached the BUFFER_MULTIPLIER, the next read will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    if (_readIndex >= BUFFER_BYTES) {
        _readIndex = 0;
        _writingLoopsAhead--;
        readingLooped = true;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy

            if (readingLooped) {
                size_t preLoop = n - _readIndex; // The number of bytes at the end of _buffer to write before it loops
                memcpy(buffer          , start  , preLoop    * sizeof(float));
                memcpy(buffer + preLoop, _buffer, _readIndex * sizeof(float));
            } else {
                memcpy(buffer          , start  , n          * sizeof(float));
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - _readIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop   ; ++i) buffer[i]             = start[i]   * volume;
                for (size_t i = 0; i < _readIndex; ++i) (buffer + preLoop)[i] = _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n         ; ++i) buffer[i]             = start[i]   * volume;
            }

        }
    } else {
        if (volume == 1.0F) {

            if (readingLooped) {
                size_t preLoop = n - _readIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop   ; ++i) buffer[i]             += start[i]  ;
                for (size_t i = 0; i < _readIndex; ++i) (buffer + preLoop)[i] += _buffer[i];
            } else {
                for (size_t i = 0; i < n         ; ++i) buffer[i]             += start[i]  ;
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - _readIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop   ; ++i) buffer[i]             += start[i]   * volume;
                for (size_t i = 0; i < _readIndex; ++i) (buffer + preLoop)[i] += _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n         ; ++i) buffer[i]             += start[i]   * volume;
            }

        }
    }

    return n;
}

bool TempBuffer::readCaughtUp() {
    // If the _readIndex has caught up fully with _writeIndex
    // Note: _readIndex should never be > _writeIndex, we want this to stop
    // when they are equal, but JUST in case, we use >=
    return _writingLoopsAhead == 0 && _readIndex >= _writeIndex;
}

bool TempBuffer::writingAhead() {
    return _writingLoopsAhead > 0 || _writeIndex > _readIndex;
}

void TempBuffer::clear() {
    _writeIndex = 0;
    _readIndex = 0;
    _writingLoopsAhead = 0;
}
