#include "audioobjectstream.h"

AudioObjectStream::AudioObjectStream() {}
AudioObjectStream::~AudioObjectStream() {}

void AudioObjectStream::setVolume(const float volume) {
    _volume = volume;
}

void AudioObjectStream::mix(float* buffer, size_t framesPerBuffer, size_t channels, int deviceListIndex, float deviceVolume, bool singleDevice) {

}
