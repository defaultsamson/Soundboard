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
    void init(const PaDeviceInfo *info);

private:
    QScopedPointer<QAudioOutput> output;
    const PaDeviceInfo *info;
    SndfileHandle file;
    bool paused = false;
};

#endif // AUDIOOBJECT_H
