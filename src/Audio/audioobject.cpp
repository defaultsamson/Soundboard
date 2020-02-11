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
    delete [] bufferRead;
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
    readStop = false;
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

void AudioObject::mix(float* buffer, size_t /*framesPerBuffer*/, int deviceListIndex, float deviceVolume, bool singleDevice) {
    if (!file) return; // If there's no file
    // If it's paused or stopped
    if (paused || stopped) return;
    if (readStop && deviceListIndex == 0) return;

    size_t frames = AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS;
    float finalVolume = _volume * deviceVolume;

    // If it's the first device, then load up the sideBuffer
    if (deviceListIndex == 0) {
        float mixBuffer[frames * sizeof(float)];

        safeRead.lock();
        size_t read = static_cast<size_t>(file->read(mixBuffer, static_cast<sf_count_t>(frames)));
        safeRead.unlock();
        bufferRead[sideBufferWrite] = read;

        size_t start = frames * sideBufferWrite;
        if (!singleDevice) memcpy(sideBuffer + start, buffer, read * sizeof(float));
        for (size_t i = 0; i < read; i++){
            buffer[i] += mixBuffer[i] * finalVolume;
        }
        if (sideBufferWrite++ >= SIDE_BUFFER_MULTIPLIER) {
            if (!singleDevice) readLoopsAhead++;
            sideBufferWrite = 0;
        }

        // Ran out of things to read, the file stream is over
        if (read == 0 || read < frames) {
            if (singleDevice) stop();
            else readStop = true;
        }
    } else {
        size_t start = frames * sideBufferRead;
        // memcpy(buffer, sideBuffer + start, bufferRead[sideBufferRead] * sizeof (float));
        for (size_t i = 0; i < bufferRead[sideBufferRead]; i++){
            buffer[i] += sideBuffer[start + i] * finalVolume;
        }
        // TODO? memset(sideBuffer + start, 0, bufferRead[sideBufferRead] * sizeof (float));
        if (sideBufferRead++ >= SIDE_BUFFER_MULTIPLIER) {
            sideBufferRead = 0;
            readLoopsAhead--;
        }
        // If it stopped reading the file, and the other device has caught up
        if (readStop && readLoopsAhead == 0 && sideBufferRead >= sideBufferWrite) {
            stop();
        }
    }
}

void AudioObject::setFile(const QString &filename) {
    stop();
    if (file) delete file;
    paused = false;
    stopped = true;

    sideBuffer = new float[SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float)];
    memset(sideBuffer, 0, SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(float));

    bufferRead = new size_t[SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(size_t)];
    memset(bufferRead, 0, SIDE_BUFFER_MULTIPLIER * AudioEngine::FRAMES_PER_BUFFER * AudioEngine::CHANNELS * sizeof(size_t));

    sideBufferWrite = 0;
    sideBufferRead = 0;
    file = new SndfileHandle(filename.toStdString());
    /*
    std::cout << "Initializing File: " << filename.toStdString() << std::endl;
    std::cout << "Sample rate:       " << file->samplerate() << std::endl;
    std::cout << "Channels:          " << file->channels() << std::endl;
    */
}

bool AudioObject::hasFile() {
    return file;
}

void AudioObject::setVolume(const float volume) {
    _volume = volume;
}
