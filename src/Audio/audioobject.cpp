#include "audioobject.h"

#include <QAudioFormat>
#include <QAudioOutput>
#include <QString>

#include <iostream>

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

void AudioObject::mix(float* buffer, size_t framesPerBuffer) {
    if (!file) return;
    size_t channels = 2; // TODO

    float mixBuffer[framesPerBuffer * channels];
    file->read(mixBuffer, framesPerBuffer * channels);

    std::cout << "2" << std::endl;

    for (int i = 0; i < framesPerBuffer * channels; i += channels) {
        buffer[i] += mixBuffer[i]; // Left
        buffer[i + 1] += mixBuffer[i + 1]; // Right
        /*
        buffer[i] = 0; // Left
        buffer[i + 1] = 0; // Right
        */
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    file = new SndfileHandle(filename.toStdString());
    std::cout << "Initializing File: " << filename.toStdString() << std::endl;

    std::cout << "Sample rate:       " << file->samplerate() << std::endl;
    std::cout << "Channels:          " << file->channels() << std::endl;
    /*
    file.close();
    file.setFileName(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        // TODO error message for could not open file
        return;
    }*/
}
