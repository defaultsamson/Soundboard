#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>
#include <mutex>

class AudioObject
{
public:
    AudioObject();
    ~AudioObject();
    void setFile(const QString &filePath);
    void setVolume(const float volume);

    void play();
    void stop();
    void pause();

    void mix(float* buffer, size_t framesPerBuffer, int deviceListIndex, float deviceVolume, bool singleDevice);

    bool isPlaying();
    bool isPaused();
    bool isStopped();

    bool hasFile();

private:
    SndfileHandle *file = nullptr;
    bool paused = false;
    bool stopped = true;
    float _volume = 1; // 0.0 - 1.0
    float *sideBuffer = nullptr;
    size_t *bufferRead = nullptr;

    // Sometimes there are 4 or possibly more reads and writes done consecutively.
    // When that happens, we need to make sure that there's enough of a buffer for
    // all of those consecutive reads, and fresh space for all the new writes.
    // When writing to the audio buffer
    const size_t SIDE_BUFFER_MULTIPLIER = 64;
    size_t sideBufferWrite = 0;
    size_t sideBufferRead = 0;
    std::mutex safeRead;
    bool device0Finished = false;
    size_t device0LoopsAhead = 0;
};

#endif // AUDIOOBJECT_H
