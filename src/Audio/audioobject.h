#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <QObject>
#include "tempbuffer.h"

class AudioObject : public QObject {

    Q_OBJECT

public:
    AudioObject();
    virtual ~AudioObject();
    int volumeInt() { return _volumeInt; }
    virtual void write(const float* buffer, size_t n) = 0;
    virtual size_t read(float* buffer, size_t n) = 0;
    virtual void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);
    virtual void stop();
    bool isStopped() { return stopped; }
    virtual bool doMix() { return true; }
    void setUpdateVisualizer(bool update) { _updateVisualiser = update; }

public slots:
    void setVolume(const float volume) { _volume = volume; }
    void setVolumeInt(const int volume) { _volumeInt = volume; setVolume(volume / static_cast<float>(100)); }

protected:
    bool stopped = true;

private:
    int _volumeInt = 100; // 0 - 100
    float _volume = 1; // 0.0 - 1.0

    TempBuffer tempBuffer;

    bool device0Finished = false;
    bool _updateVisualiser = false;

signals:
    void update(float level);
};

#endif // AUDIOOBJECT_H
