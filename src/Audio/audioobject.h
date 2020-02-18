#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <stddef.h>

class AudioObject {
public:
    AudioObject();
    virtual ~AudioObject();
    void setVolume(const float volume);
    virtual void write(const float* buffer, size_t n) = 0;
    virtual size_t read(float* buffer, size_t n) = 0;
    virtual void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);
    virtual void stop();
    bool isStopped() { return stopped; }
    virtual bool doMix() { return true; }

protected:
    bool stopped = true;

    // Sometimes there are 4 or possibly more reads and writes done consecutively.
    // When that happens, we need to make sure that there's enough of a buffer for
    // all of those consecutive reads, and fresh space for all the new writes.
    // When writing to the audio buffer
    const size_t SIDE_BUFFER_MULTIPLIER = 64;
    float* sideBuffer = nullptr;

private:
    float _volume = 1; // 0.0 - 1.0
    size_t* bytesRead = nullptr;
    size_t sideBufferWrite = 0;
    size_t sideBufferRead = 0;
    bool device0Finished = false;
    size_t device0LoopsAhead = 0;
};

#endif // AUDIOOBJECT_H
