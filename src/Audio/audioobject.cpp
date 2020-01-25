#include "audioobject.h"
#include "audiofilestream.h"

#include <QAudioFormat>
#include <QAudioOutput>
#include <QString>
#include <QObject>

#include <portaudio.h>
#include <sndfile.hh>

AudioObject::AudioObject()
{

}

void AudioObject::init(const PaDeviceInfo* info) {
    // TODO
    //if (output) output->stop();
    //_stream.stop();
    /*QAudioFormat format = info.preferredFormat();
    if (!_stream.init(format)) {
        qWarning() << "Failed to init audio stream";
        // TODO error message to user
        return;
    }
    output.reset(new QAudioOutput(info, format));
    output->start(&_stream);*/
}

AudioFileStream &AudioObject::stream() {
    //return _stream;
}

void AudioObject::stop() {
    //_stream.stop();
    paused = false;
}

void AudioObject::pause() {
    //output->suspend();
    paused = true;
}

void AudioObject::play() {
    if (paused) {
        //output->resume();
        paused = false;
    } else {
        // plays from beginning of file
        //_stream.stop();
        //_stream.play(file);
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    file = SndfileHandle(filename.toStdString());
    qDebug() << "Loading file: " << filename;
    /*
    file.close();
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        // TODO error message for could not open file
        return;
    }*/
}
