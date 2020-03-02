#include "tempbuffer.h"

#include <string.h>

TempBuffer::TempBuffer() {
    // Set up _buffer to hold as much data as a device's total requested frames (frames per buffer * channels), times the BUFFER_MULTIPLIER
    _buffer = new float[BUFFER_BYTES];
    clear();
}

TempBuffer::~TempBuffer() {
    delete [] _buffer;
}

void TempBuffer::forwardWriteIndex(size_t n) {
    _writeIndex += n;
    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    // WARNING: This assumes that _writeIndex can only loop ahead at max 1 time
    if (_writeIndex >= BUFFER_BYTES) {
        _writingLoopsAhead++;
        _writeIndex -= BUFFER_BYTES;
    }
}

void TempBuffer::write(const float* buffer, size_t n, float volume, bool overwrite, bool forwardWriteIndex, bool monoToStereo) {
    float* start = _buffer + _writeIndex;

    size_t currentWriteIndex = _writeIndex + n;
    if (forwardWriteIndex) {
        // We are essentially doubling the n for the write index when
        // buffer is mono, that way we can write to _buffer as stereo
        if (monoToStereo) _writeIndex = currentWriteIndex + n;
        else _writeIndex = currentWriteIndex;
    }

    bool writingLooped = false;

    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    // WARNING: This assumes that _writeIndex can only loop ahead at max 1 time
    if (currentWriteIndex >= BUFFER_BYTES) {
        if (forwardWriteIndex) {
            _writingLoopsAhead++;
            _writeIndex -= BUFFER_BYTES;
        }
        currentWriteIndex -= BUFFER_BYTES;
        writingLooped = true;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy

            if (writingLooped) {
                size_t preLoop = n - currentWriteIndex; // The number of bytes at the end of _buffer to write before it loops
                const float* nextBuffer = buffer + preLoop;
                if (monoToStereo) {
                    for (size_t i = 0; i < preLoop; ++i) {
                        start[(i * 2)]     = buffer[i];
                        start[(i * 2) + 1] = buffer[i];
                    }
                    for (size_t i = 0; i < currentWriteIndex; ++i) {
                        _buffer[(i * 2)]     = nextBuffer[i];
                        _buffer[(i * 2) + 1] = nextBuffer[i];
                    }
                } else {
                    memcpy(start  , buffer          , preLoop           * sizeof(float));
                    memcpy(_buffer, nextBuffer, currentWriteIndex * sizeof(float));
                }
            } else {
                if (monoToStereo) {
                    for (size_t i = 0; i < n; ++i) {
                        start[(i * 2)]     = buffer[i];
                        start[(i * 2) + 1] = buffer[i];
                    }
                } else {
                    memcpy(start, buffer, n * sizeof(float));
                }
            }

        } else {

            if (writingLooped) {
                size_t preLoop = n - currentWriteIndex; // The number of bytes at the end of _buffer to write before it loops
                const float* nextBuffer = buffer + preLoop;
                if (monoToStereo) {
                    for (size_t i = 0; i < preLoop; ++i) {
                        float final = buffer[i] * volume;
                        start[(i * 2)]     = final;
                        start[(i * 2) + 1] = final;
                    }
                    for (size_t i = 0; i < currentWriteIndex; ++i) {
                        float final = nextBuffer[i] * volume;
                        _buffer[(i * 2)]     = final;
                        _buffer[(i * 2) + 1] = final;
                    }
                } else {
                    for (size_t i = 0; i < preLoop          ; ++i) start[i]   = buffer[i]             * volume;
                    for (size_t i = 0; i < currentWriteIndex; ++i) _buffer[i] = (buffer + preLoop)[i] * volume;
                }
            } else {
                if (monoToStereo) {
                    for (size_t i = 0; i < n; ++i) {
                        float final = buffer[i] * volume;
                        start[(i * 2)]     = final;
                        start[(i * 2) + 1] = final;
                    }
                } else {
                    for (size_t i = 0; i < n; ++i) start[i] = buffer[i] * volume;
                }
            }

        }
    } else {
        if (volume == 1.0F) {

            if (writingLooped) {
                size_t preLoop = n - currentWriteIndex; // The number of bytes at the end of _buffer to write before it loops
                const float* nextBuffer = buffer + preLoop;
                if (monoToStereo) {
                    for (size_t i = 0; i < preLoop; ++i) {
                        start[(i * 2)]     += buffer[i];
                        start[(i * 2) + 1] += buffer[i];
                    }
                    for (size_t i = 0; i < currentWriteIndex; ++i) {
                        _buffer[(i * 2)]     += nextBuffer[i];
                        _buffer[(i * 2) + 1] += nextBuffer[i];
                    }
                } else {
                    for (size_t i = 0; i < preLoop          ; ++i) start[i]   += buffer[i]    ;
                    for (size_t i = 0; i < currentWriteIndex; ++i) _buffer[i] += nextBuffer[i];
                }
            } else {

                if (monoToStereo) {
                    for (size_t i = 0; i < n; ++i) {
                        start[(i * 2)]     += buffer[i];
                        start[(i * 2) + 1] += buffer[i];
                    }
                } else {
                    for (size_t i = 0; i < n; ++i) start[i] += buffer[i];
                }
            }

        } else {

            if (writingLooped) {
                size_t preLoop = n - currentWriteIndex; // The number of bytes at the end of _buffer to write before it loops
                const float* nextBuffer = buffer + preLoop;
                if (monoToStereo) {
                    for (size_t i = 0; i < preLoop; ++i) {
                        float final = buffer[i] * volume;
                        start[(i * 2)]     += final;
                        start[(i * 2) + 1] += final;
                    }
                    for (size_t i = 0; i < currentWriteIndex; ++i) {
                        float final = nextBuffer[i] * volume;
                        _buffer[(i * 2)]     += final;
                        _buffer[(i * 2) + 1] += final;
                    }
                } else {
                    for (size_t i = 0; i < preLoop          ; ++i) start[i]   += buffer[i]     * volume;
                    for (size_t i = 0; i < currentWriteIndex; ++i) _buffer[i] += nextBuffer[i] * volume;
                }
            } else {
                if (monoToStereo) {
                    for (size_t i = 0; i < n; ++i) {
                        float final = buffer[i] * volume;
                        start[(i * 2)]     += final;
                        start[(i * 2) + 1] += final;
                    }
                } else {
                    for (size_t i = 0; i < n; ++i) start[i] += buffer[i] * volume;
                }
            }
        }
    }
}

size_t TempBuffer::read(float* buffer, size_t n, float volume, bool overwrite, bool forwardReadIndex) {
    // TODO return when there's < n bytes to read

    // Ensures that the writing is always ahead of the reading
    if (!writingAhead() || n == 0) return 0;

    float* start = _buffer + _readIndex;

    // Compute the max n that can be read
    size_t maxN = availableRead();
    n = maxN > n ? n : maxN;
    if (n == 0) return 0;

    // Then process the read
    size_t currentReadIndex = _readIndex + n;
    if (forwardReadIndex) _readIndex = currentReadIndex;
    bool readingLooped = false;

    // If the _readIndex has reached the BUFFER_MULTIPLIER, the next read will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    if (currentReadIndex >= BUFFER_BYTES) {
        if (forwardReadIndex) {
            _writingLoopsAhead--;
            _readIndex -= BUFFER_BYTES;
        }
        currentReadIndex -= BUFFER_BYTES;
        readingLooped = true;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy

            if (readingLooped) {
                size_t preLoop = n - currentReadIndex; // The number of bytes at the end of _buffer to write before it loops
                memcpy(buffer          , start  , preLoop          * sizeof(float));
                memcpy(buffer + preLoop, _buffer, currentReadIndex * sizeof(float));
            } else {
                memcpy(buffer          , start  , n                * sizeof(float));
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - currentReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop         ; ++i) buffer[i]             = start[i]   * volume;
                for (size_t i = 0; i < currentReadIndex; ++i) (buffer + preLoop)[i] = _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n               ; ++i) buffer[i]             = start[i]   * volume;
            }

        }
    } else {
        if (volume == 1.0F) {

            if (readingLooped) {
                size_t preLoop = n - currentReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop         ; ++i) buffer[i]             += start[i]  ;
                for (size_t i = 0; i < currentReadIndex; ++i) (buffer + preLoop)[i] += _buffer[i];
            } else {
                for (size_t i = 0; i < n               ; ++i) buffer[i]             += start[i]  ;
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - currentReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop         ; ++i) buffer[i]             += start[i]   * volume;
                for (size_t i = 0; i < currentReadIndex; ++i) (buffer + preLoop)[i] += _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n               ; ++i) buffer[i]             += start[i]   * volume;
            }

        }
    }

    return n;
}

size_t TempBuffer::availableRead() {
    size_t maxN = 0;
    // Compute the max n that can be read
    if (_writingLoopsAhead > 0) {
        // If the writing is a loop ahead, the available bytes in _buffer to be read is
        // _buffer = [0 ... <-_writeIndex ... _readIndex-> ... (BUFFER_BYTES - 1)]
        maxN = _writeIndex + (BUFFER_BYTES - _readIndex);
    } else {
        // _buffer = [0 ... _readIndex-> ... <-_writeIndex ... (BUFFER_BYTES - 1)]
        maxN = _writeIndex - _readIndex;
    }
    return maxN;
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
    memset(_buffer, 0, BUFFER_BYTES * sizeof(float));
}
