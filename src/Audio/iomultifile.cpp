#include "iomultifile.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <samplerate.h>
#include <iostream>
#include "../Audio/audioengine.h"

IOMultiFile::~IOMultiFile() {
    clear();
}

void IOMultiFile::write(const float* /*buffer*/, size_t /*n*/) {

}
size_t IOMultiFile::read(float *buffer, size_t n) {
    memset(buffer, 0, n * sizeof(float));
    return mix(buffer, n);
}
size_t IOMultiFile::mix(float* buffer, size_t n) {
    float inBuffer[n];
    float outBuffer[n];
    long maxRead = 0;

    // Makes it only read what it needs to
    if (mono) n /= 2;

    data.data_in = inBuffer;
    data.data_out = outBuffer;

    // Iterates the open files and adds their read bytes into buffer
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {

        long read = _openFiles.at(i)->read(inBuffer, static_cast<sf_count_t>(n));

        data.input_frames = read / _channels;
        data.output_frames = n / _channels;
        int err = src_process(state, &data);
        if (err != 0) continue; // TODO ERROR

        for (size_t i = 0; i < read; i++) {
            if (mono) {
                buffer[(i * 2)] += outBuffer[i];
                buffer[(i * 2) + 1] += outBuffer[i];
            }
            else buffer[i] += outBuffer[i];
        }
        if (read > maxRead) maxRead = read;
        if (read < n) delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();

    return mono ? maxRead * 2 : maxRead;
}

void IOMultiFile::startRead() {
    modifyLock.lock();
    _openFiles.append(new SndfileHandle(_filename));
    modifyLock.unlock();
}

void IOMultiFile::setFile(std::string filename) {
    clear();
    _filename = filename;
    SndfileHandle tempFile(_filename);
    _channels = tempFile.channels();
    mono = _channels == 1;
    int error = 0;
    state = src_new(SRC_SINC_MEDIUM_QUALITY, _channels, &error);
    if (error != 0) return; // TODO ERROR
    data.src_ratio = static_cast<double>(SAMPLE_RATE) / static_cast<double>(tempFile.samplerate());
}

void IOMultiFile::clear() {
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();
}

