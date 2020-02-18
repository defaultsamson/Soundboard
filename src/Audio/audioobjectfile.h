#ifndef AUDIOOBJECTFILE_H
#define AUDIOOBJECTFILE_H

#include <QString>

#include "audioobject.h"
#include "iomultifile.h"
#include <portaudio.h>
#include <sndfile.hh>
#include <string>
#include <mutex>

class AudioObjectFile : public AudioObject
{
public:
    AudioObjectFile();
    ~AudioObjectFile();
    void setFile(const QString &filePath);

    void play();
    void stop();
    void pause();

    void setVolume(const float volume);
    void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);

    bool isPlaying();
    bool isPaused();
    bool isStopped();

    bool hasFile();

private:
    IOMultiFile _file;
    std::string _filename;
    bool _hasFile = false;
    bool paused = false;
    bool stopped = true;
    float _volume = 1; // 0.0 - 1.0
    float* sideBuffer = nullptr;
    size_t* bytesRead = nullptr;

    // Sometimes there are 4 or possibly more reads and writes done consecutively.
    // When that happens, we need to make sure that there's enough of a buffer for
    // all of those consecutive reads, and fresh space for all the new writes.
    // When writing to the audio buffer
    const size_t SIDE_BUFFER_MULTIPLIER = 64;
    size_t sideBufferWrite = 0;
    size_t sideBufferRead = 0;
    bool device0Finished = false;
    size_t device0LoopsAhead = 0;
};

#endif // AUDIOOBJECTFILE_H
