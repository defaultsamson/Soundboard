#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QAudioOutput>
#include <QFile>

#include <portaudio.h>

#include "audiofilestream.h"

class AudioObject
{
public:
    AudioObject();
    void setFile(const QString &filePath);
    void play();
    void stop();
    void pause();
    void init(const PaDeviceInfo *info);
    AudioFileStream &stream();

private:
    QScopedPointer<QAudioOutput> output;
    AudioFileStream _stream;
    const PaDeviceInfo *info;
    QFile file;
    bool paused = false;
};

#endif // AUDIOOBJECT_H
