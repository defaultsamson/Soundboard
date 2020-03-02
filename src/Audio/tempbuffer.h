#ifndef TEMPBUFFER_H
#define TEMPBUFFER_H

#include <stddef.h>

class TempBuffer
{
public:
    TempBuffer();
    ~TempBuffer();

    void write(const float* buffer, size_t n, float volume = 1.0, bool overwrite = true);
    size_t read(float* buffer, size_t n, float volume = 1.0, bool overwrite = true);

    bool readCaughtUp();
    bool writingAhead();

    void clear();

private:
    // Sometimes there are 4 or possibly more reads and writes done consecutively.
    // When that happens, we need to make sure that there's enough of a buffer for
    // all of those consecutive reads, and fresh space for all the new writes.
    // When writing to the audio buffer
    const size_t FRAMES = 256 * 2; // AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;
    const size_t BUFFER_BYTES = FRAMES * 64;

    float* _buffer = nullptr;
    size_t _writeIndex = 0;
    size_t _readIndex = 0;
    size_t _writingLoopsAhead = 0;
};

#endif // TEMPBUFFER_H
