#ifndef AUDIOOBJECTINPUT_H
#define AUDIOOBJECTINPUT_H

#include "audioobject.h"

class AudioObjectInput  : public AudioObject
{
public:
    AudioObjectInput();
    ~AudioObjectInput();

    void write(const float* buffer, size_t n);
    size_t read(float* buffer, size_t n);
    bool doMix();
    void mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice);

    void setOutput0(bool active) { _output0 = active; }
    bool isActiveOutput0() { return _output0; }
    void setOutput1(bool active) { _output1 = active; }
    bool isActiveOutput1() { return _output1; }

private:
    float *inBuffer = nullptr;
    size_t* bytesWritten = nullptr;
    size_t inBufferWrite = 0;
    size_t inBufferRead = 0;
    size_t inBufferLoopsAhead = 0;

    bool _output0 = false;
    bool _output1 = false;
};

#endif // AUDIOOBJECTINPUT_H
