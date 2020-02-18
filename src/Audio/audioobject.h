#ifndef AUDIOOBJECT_H
#define AUDIOOBJECT_H

#include <stddef.h>

class AudioObject {
public:
    virtual ~AudioObject() {}
    virtual void setVolume(const float volume) = 0;
    virtual void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice) = 0;
};

#endif // AUDIOOBJECT_H
