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

void TempBuffer::forwardReadIndex(size_t n) {
    _readIndex += n;
    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    // WARNING: This assumes that _writeIndex can only loop ahead at max 1 time
    if (_readIndex >= BUFFER_BYTES) {
        _writingLoopsAhead--;
        _readIndex -= BUFFER_BYTES;
    }
}

// Applies the volume to the next n bytes
void TempBuffer::applyVolume(size_t n, float volume) {
    for (size_t i = 0; i < n; ++i) _buffer[i] = _buffer[i] * volume;
}

// Doubles the next n bytes
// This requires write() to be called with forwardWriteIndex = false
size_t TempBuffer::monoToStereo(size_t n) {
    // The number of bytes at the end of _buffer to write before it loops
    size_t bytesBeforeLoop = BUFFER_BYTES - _writeIndex;
    float* start = _buffer + _writeIndex;
    size_t OUTPUT_LENGTH = n * 2;

    // If the conversion output would go beyond the length of _buffer
    if (OUTPUT_LENGTH > bytesBeforeLoop) {
        // If the number of bytes to write will require a loop around,
        // fill a temp buffer and overwrite the existing bytes.
        float* temp = new float[OUTPUT_LENGTH];

        // If the conversion input goes beyond the length of _buffer
        if (n > bytesBeforeLoop) {
            size_t newI;
            for (size_t i = 0; i < bytesBeforeLoop; ++i) {
                newI = i * 2;
                temp[newI]     = start[i];
                temp[newI + 1] = start[i];
            }
            for (size_t i = 0; i < n - bytesBeforeLoop; ++i) {
                newI = (i + bytesBeforeLoop) * 2;
                temp[newI]     = _buffer[i];
                temp[newI + 1] = _buffer[i];
            }
        } else {
            size_t newI;
            for (size_t i = 0; i < n; ++i) {
                newI = i * 2;
                temp[newI]     = start[i];
                temp[newI + 1] = start[i];
            }
        }

        write(temp, n * 2, true, false);

        delete [] temp;

    } else {
        // if n doesn't loop
        for (int i = static_cast<int>(n - 1); i >= 0; --i) {
            start[(i * 2)]     = start[i];
            start[(i * 2) + 1] = start[i];
        }
    }

    return n * 2;
}

void TempBuffer::write(const float* buffer, size_t n, bool overwrite, bool forwardWriteIndex) {
    float* start = _buffer + _writeIndex;

    size_t endWriteIndex = _writeIndex + n;
    if (forwardWriteIndex)
        _writeIndex = endWriteIndex;

    bool writingLooped = false;

    // If the _writeIndex has reached the BUFFER_MULTIPLIER, the next write will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    // WARNING: This assumes that _writeIndex can only loop ahead at max 1 time
    if (endWriteIndex >= BUFFER_BYTES) {
        if (forwardWriteIndex) {
            _writingLoopsAhead++;
            _writeIndex -= BUFFER_BYTES;
        }
        endWriteIndex -= BUFFER_BYTES;
        writingLooped = true;
    }

    if (overwrite) {
        if (writingLooped) {
            size_t preLoop = n - endWriteIndex; // The number of bytes at the end of _buffer to write before it loops
            const float* nextBuffer = buffer + preLoop;
            memcpy(start  , buffer          , preLoop       * sizeof(float));
            memcpy(_buffer, nextBuffer      , endWriteIndex * sizeof(float));
        } else {
            memcpy(start  , buffer          , n             * sizeof(float));
        }
    } else {
        if (writingLooped) {
            size_t preLoop = n - endWriteIndex; // The number of bytes at the end of _buffer to write before it loops
            const float* nextBuffer = buffer + preLoop;
            for (size_t i = 0; i < preLoop      ; ++i) start[i]   += buffer[i]    ;
            for (size_t i = 0; i < endWriteIndex; ++i) _buffer[i] += nextBuffer[i];
        } else {

            for (size_t i = 0; i < n            ; ++i) start[i]   += buffer[i]    ;
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
    size_t endReadIndex = _readIndex + n;
    if (forwardReadIndex) _readIndex = endReadIndex;
    bool readingLooped = false;

    // If the _readIndex has reached the BUFFER_MULTIPLIER, the next read will
    // surpass the limit of _buffer. To avoid this, we loop it back around to the beginning
    if (endReadIndex >= BUFFER_BYTES) {
        if (forwardReadIndex) {
            _writingLoopsAhead--;
            _readIndex -= BUFFER_BYTES;
        }
        endReadIndex -= BUFFER_BYTES;
        readingLooped = true;
    }

    if (overwrite) {
        if (volume == 1.0F) {
            // No volume changes to make, write using memcpy

            if (readingLooped) {
                size_t preLoop = n - endReadIndex; // The number of bytes at the end of _buffer to write before it loops
                memcpy(buffer          , start  , preLoop      * sizeof(float));
                memcpy(buffer + preLoop, _buffer, endReadIndex * sizeof(float));
            } else {
                memcpy(buffer          , start  , n            * sizeof(float));
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - endReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop     ; ++i) buffer[i]             = start[i]   * volume;
                for (size_t i = 0; i < endReadIndex; ++i) (buffer + preLoop)[i] = _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n           ; ++i) buffer[i]             = start[i]   * volume;
            }

        }
    } else {
        if (volume == 1.0F) {

            if (readingLooped) {
                size_t preLoop = n - endReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop     ; ++i) buffer[i]             += start[i]  ;
                for (size_t i = 0; i < endReadIndex; ++i) (buffer + preLoop)[i] += _buffer[i];
            } else {
                for (size_t i = 0; i < n           ; ++i) buffer[i]             += start[i]  ;
            }

        } else {

            if (readingLooped) {
                size_t preLoop = n - endReadIndex; // The number of bytes at the end of _buffer to write before it loops
                for (size_t i = 0; i < preLoop     ; ++i) buffer[i]             += start[i]   * volume;
                for (size_t i = 0; i < endReadIndex; ++i) (buffer + preLoop)[i] += _buffer[i] * volume;
            } else {
                for (size_t i = 0; i < n           ; ++i) buffer[i]             += start[i]   * volume;
            }

        }
    }

    return n;
}

size_t TempBuffer::applySampleRateChange(size_t n, size_t channels, SRC_STATE* state, SRC_DATA& data) {
    if (data.src_ratio == 1.0) return n;

    float* inBuffer = nullptr;

    // If reading n bytes from _buffer would go past BUFFER_BYTES
    if (n + _writeIndex > BUFFER_BYTES) {
        // Fill a temporary inBuffer
        inBuffer = new float[n];
        data.data_in = inBuffer;

        size_t preLoop = BUFFER_BYTES - _writeIndex;
        memcpy(inBuffer          , _buffer + _writeIndex, preLoop       * sizeof(float));
        memcpy(inBuffer + preLoop, _buffer              , (n - preLoop) * sizeof(float));

    } else {
        data.data_in = _buffer + _writeIndex;
    }

    // TODO what if the outBuffer is overfilled? e.g. if input_frames_used != n
    // Should we consider a scenario where outBuffer is also _buffer + writeIndex, as we did with inBuffer??
    float* outBuffer = new float[n];
    data.data_out = outBuffer;

    data.input_frames = n / channels;
    data.output_frames = n / channels;

    // TODO ERROR int err =
    src_process(state, &data);
    size_t converted = static_cast<size_t>(data.output_frames_gen) * channels;
    // TODO ERROR if (err != 0) ;

    write(outBuffer, converted, true, false);

    if (inBuffer) delete [] inBuffer;
    delete [] outBuffer;

    return converted;
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
