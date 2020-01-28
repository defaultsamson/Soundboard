#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>

struct DeviceInfoContainer {
    const PaDeviceInfo *info;
    PaDeviceIndex index;

};

class AudioObject
{
public:
    AudioObject();
    void setFile(const QString &filePath);
    void setVolume(const float volume);

    void play();
    void stop();
    void pause();

    void mix(float* buffer, size_t framesPerBuffer);

    bool isPlaying();
    bool isPaused();
    bool isStopped();

private:
    SndfileHandle *file = nullptr;
    bool paused = false;
    bool stopped = true;
    float _volume = 1; // 0.0 - 1.0
};

#endif // AUDIOOBJECT_H
