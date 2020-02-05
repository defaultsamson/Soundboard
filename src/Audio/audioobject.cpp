#include "audioobject.h"

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>

AudioObject::AudioObject()
{

}

bool AudioObject::isPlaying() { return !isPaused() && !isStopped(); }
bool AudioObject::isPaused() { return paused; }
bool AudioObject::isStopped() { return stopped; }

void AudioObject::stop() {
    if (!file) return;
    // std::cout << "Stopping" << std::endl;
    paused = false;
    stopped = true;
}

void AudioObject::pause() {
    if (!file || stopped) return;
    // std::cout << "Pausing" << std::endl;
    paused = true;
    stopped = false;
}

void AudioObject::play() {
    if (!file) return;
    // std::cout << "Playing" << std::endl;
    if (paused) {
        // Resume
        paused = false;
    } else if (stopped) {
        // Play from beginning of file
        stopped = false;
        file->seek(0, SEEK_SET);
    }
}

void AudioObject::mix(float* buffer, size_t framesPerBuffer) {
    if (!file || paused || stopped) return;
    size_t channels = 2; // TODO

    size_t frames = framesPerBuffer * channels;

    float *mixBuffer = static_cast<float*>(malloc(frames * sizeof(float)));
    sf_count_t read = file->read(mixBuffer, frames);

    for (int i = 0; i < read; i += channels) {
        buffer[i] += mixBuffer[i] * _volume; // Left
        buffer[i + 1] += mixBuffer[i + 1] * _volume; // Right
    }

    free(mixBuffer);

    // Ran out of things to read, the file stream is over
    if (read == 0 || read < frames) {
        stop();
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    if (file) delete file;
    paused = false;
    stopped = true;
    file = new SndfileHandle(filename.toStdString());
    /*
    std::cout << "Initializing File: " << filename.toStdString() << std::endl;
    std::cout << "Sample rate:       " << file->samplerate() << std::endl;
    std::cout << "Channels:          " << file->channels() << std::endl;
    */
}

void AudioObject::setVolume(const float volume) {
    _volume = volume;
}
