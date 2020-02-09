#include "audioobject.h"

#include <QString>

#include <portaudio.h>
#include <sndfile.hh>
#include "audioengine.h"

AudioObject::AudioObject()
{

}
AudioObject::~AudioObject() {
    delete [] sideBuffer;
    delete file;
}

bool AudioObject::isPlaying() { return !isPaused() && !isStopped(); }
bool AudioObject::isPaused() { return paused; }
bool AudioObject::isStopped() { return stopped; }

void AudioObject::stop() {
    if (!file) return;
    // std::cout << "Stopping" << std::endl;
    paused = false;
    stopped = true;
    // We can never seek whilst reading
    safeRead.lock();
    file->seek(0, SEEK_SET);
    safeRead.unlock();
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
        // std::cout << "Playing" << std::endl;
    }
}

void AudioObject::mix(float* buffer, size_t /*framesPerBuffer*/, int deviceListIndex) {
    if (!file) return; // If there's no file
    // If it's paused or stopped, and the buffer write and read scanner are equal
    if ((paused || stopped) && sideBufferWrite == sideBufferRead) return;

    size_t frames = AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;

    // If it's the first device, then load up the sideBuffer
    if (deviceListIndex == 0) {
        float mixBuffer[frames * sizeof(float)];

        safeRead.lock();
        size_t read = static_cast<size_t>(file->read(mixBuffer, static_cast<sf_count_t>(frames)));
        safeRead.unlock();
        bufferRead = read;

        size_t start = frames * sideBufferWrite;
        for (size_t i = 0; i < read; i++){
            buffer[i] += mixBuffer[i] * _volume;
        }
        memcpy(sideBuffer + start, buffer, read * sizeof(float));
        if (sideBufferWrite++ >= SIDE_BUFFER_MULTIPLIER) sideBufferWrite = 0;

        // Ran out of things to read, the file stream is over
        if (read == 0 || read < frames) {
            stop();
        }
    } else {
        size_t start = frames * sideBufferRead;
        memcpy(buffer, sideBuffer + start, bufferRead * sizeof (float));
        memset(sideBuffer + start, 0, bufferRead * sizeof (float));
        if (sideBufferRead++ >= SIDE_BUFFER_MULTIPLIER) sideBufferRead = 0;
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    if (file) delete file;
    paused = false;
    stopped = true;
    sideBuffer = new float[SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float)];
    memset(sideBuffer, 0, SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float));
    bufferRead = 0;
    sideBufferWrite = 0;
    sideBufferRead = 0;
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
