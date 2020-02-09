#include "audioobject.h"

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>
#include "audioengine.h"

AudioObject::AudioObject()
{

}
AudioObject::~AudioObject() {
    free(sideBuffer);
    delete file0;
    delete file1;
}

bool AudioObject::isPlaying() { return !isPaused() && !isStopped(); }
bool AudioObject::isPaused() { return paused; }
bool AudioObject::isStopped() { return stopped; }

void AudioObject::stop() {
    // std::cout << "Stopping" << std::endl;
    paused = false;
    stopped = true;
}

void AudioObject::pause() {
    // std::cout << "Pausing" << std::endl;
    paused = true;
    stopped = false;
}

void AudioObject::play() {
    if (!(file0 && file1)) return;
    // std::cout << "Playing" << std::endl;
    if (paused) {
        // Resume
        paused = false;
    } else if (stopped) {
        // Play from beginning of file
        stopped = false;
        file0->seek(0, SEEK_SET);
        file1->seek(0, SEEK_SET);
    }
}

void AudioObject::mix(float* buffer, size_t /*framesPerBuffer*/, int deviceListIndex) {
    if (!file0 || paused || stopped) return;

    size_t frames = AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;

    SndfileHandle *file = deviceListIndex == 0 ? file0 : file1;

    float *mixBuffer = static_cast<float*>(malloc(frames * sizeof(float)));
    sf_count_t read = file->read(mixBuffer, static_cast<sf_count_t>(frames));
    bufferRead = read;

    for (int i = 0; i < read; i ++){
        buffer[i] += mixBuffer[i] * _volume;
    }
    sideBufferScan++;
    if (sideBufferScan >= SIDE_BUFFER_MULTIPLIER) sideBufferScan = 0;

    free(mixBuffer);

    // Ran out of things to read, the file stream is over
    if (read == 0 || read < static_cast<sf_count_t>(frames)) {
        stop();
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    if (file0) delete file0;
    if (file1) delete file1;
    paused = false;
    stopped = true;
    sideBuffer = static_cast<float*>(malloc(SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float)));
    sideBufferScan = 0;
    file0 = new SndfileHandle(filename.toStdString());
    file1 = new SndfileHandle(filename.toStdString());
    /*
    std::cout << "Initializing File: " << filename.toStdString() << std::endl;
    std::cout << "Sample rate:       " << file->samplerate() << std::endl;
    std::cout << "Channels:          " << file->channels() << std::endl;
    */
}

void AudioObject::setVolume(const float volume) {
    _volume = volume;
}
