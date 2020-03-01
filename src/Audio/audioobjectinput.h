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
    bool doMix() { return !_isMuted; }
    void stop();

    void setOutput0(bool active) { _output0 = active; }
    void setHasOutput0(bool active) { _hasOutput0 = active; }
    bool isActiveOutput0() { return _output0; }

    void setOutput1(bool active) { _output1 = active; }
    void setHasOutput1(bool active) { _hasOutput1 = active; }
    bool isActiveOutput1() { return _output1; }

    void setHasInputDevice(bool active) { _hasInputDevice = active; if (!_hasInputDevice) emit update(0); }
    bool hasInputDevice() { return _hasInputDevice; }

    void setMute(bool mute) { _isMuted = mute; stop(); }
    bool isMuted() { return _isMuted; }

private:
    const size_t SIDE_BUFFER_MULTIPLIER = 64;

    float *inBuffer = nullptr;
    size_t* bytesWritten = nullptr;
    size_t inBufferWrite = 0;
    size_t inBufferRead = 0;
    size_t inBufferLoopsAhead = 0;

    bool _output0 = false;
    bool _hasOutput0 = false;
    bool _output1 = false;
    bool _hasOutput1 = false;
    bool _hasInputDevice = false;

    bool _isMuted = false;
};

#endif // AUDIOOBJECTINPUT_H
