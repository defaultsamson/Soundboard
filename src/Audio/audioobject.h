#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QObject>


class AudioObject : public QObject {

    Q_OBJECT

public:
    AudioObject();
    virtual ~AudioObject();
    void setVolumeInt(const int volume) { _volumeInt = volume; _volume = volume / static_cast<float>(100); }
    void setVolume(const float volume) { _volume = volume; }
    virtual void write(const float* buffer, size_t n) = 0;
    virtual size_t read(float* buffer, size_t n) = 0;
    virtual void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);
    virtual void stop();
    bool isStopped() { return stopped; }
    virtual bool doMix() { return true; }
    void setUpdateVisualizer(bool update) { _updateVisualiser = update; }

protected:
    bool stopped = true;

    // Sometimes there are 4 or possibly more reads and writes done consecutively.
    // When that happens, we need to make sure that there's enough of a buffer for
    // all of those consecutive reads, and fresh space for all the new writes.
    // When writing to the audio buffer
    const size_t SIDE_BUFFER_MULTIPLIER = 64;
    float* sideBuffer = nullptr;

private:
    int _volumeInt = 100; // 0 - 100
    float _volume = 1; // 0.0 - 1.0
    size_t* bytesRead = nullptr;
    size_t sideBufferWrite = 0;
    size_t sideBufferRead = 0;
    bool device0Finished = false;
    size_t device0LoopsAhead = 0;
    bool _updateVisualiser = false;

signals:
    void update(float volume);
};

#endif // AUDIOOBJECT_H
