#ifndef AUDIOOBJECTSTREAM_H
#define AUDIOOBJECTSTREAM_H

#include "audioobject.h"

class AudioObjectStream  : public AudioObject
{
public:
    AudioObjectStream();
    ~AudioObjectStream();

    void setVolume(const float volume);
    void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);

private:
    float _volume = 1; // 0.0 - 1.0
};

#endif // AUDIOOBJECTSTREAM_H
