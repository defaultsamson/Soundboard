#include "audioobject.h"

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QString>

AudioObject::AudioObject()
{

}

void AudioObject::init(const QAudioDeviceInfo &info) {
    if (output) output->stop();
    stream.stop();
    QAudioFormat format = info.preferredFormat();
    if (!stream.init(format)) {
        qWarning() << "Failed to init audio stream";
        // TODO error message to user
        return;
    }
    output.reset(new QAudioOutput(info, format));
    output->start(&stream);
}

void AudioObject::stop() {
    stream.stop();
    //output->stop();
}

// TODO test this
void AudioObject::pause() {
    output->suspend();
}

void AudioObject::play() {
    if (!paused) {
        // plays fgrom beginning of file
        qDebug() << "SHID";
        stream.stop();
    }
    stream.play(file);
}

void AudioObject::setFile(const QString &filename) {
    stop();
    file.close();
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        // TODO error message for could not open file
        return;
    }
}
