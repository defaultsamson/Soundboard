#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>

#include "audiofilestream.h"

class AudioObject
{
public:
    AudioObject();
    void setFile(const QString &filePath);
    void play();
    void stop();
    void pause();
    void init(const QAudioDeviceInfo &info);

private:
    QScopedPointer<QAudioOutput> output;
    AudioFileStream stream;
    QAudioDeviceInfo info;
    QFile file;
    bool paused = false;
};

#endif // AUDIOOBJECT_H
