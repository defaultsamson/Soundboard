#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QAudioOutput>
#include <QFile>

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
};

#endif // AUDIOOBJECT_H
