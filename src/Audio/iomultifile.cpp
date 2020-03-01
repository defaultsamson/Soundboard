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
    n = _inverseRatio * n;
    n += (n % 2); // if the number isn't even, make it even
    // std::cout << "n: " << n << std::endl;

    float inBuffer[n];
    float outBuffer[n];
    long maxRead = 0;
    // std::cout << "n1: " << n << std::endl;

    // Makes it only read what it needs to
    if (mono) n /= 2;
    // std::cout << "n3: " << n << std::endl;

    data.data_in = inBuffer;
    data.data_out = outBuffer;
    // std::cout << "n4: " << n << std::endl;

    // Iterates the open files and adds their read bytes into buffer
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        // std::cout << "n5: " << n << std::endl;

        long read = _openFiles.at(i)->read(inBuffer, static_cast<sf_count_t>(n));
        // std::cout << "n6: " << n << std::endl;

        data.input_frames = read / _channels;
        data.output_frames = n / _channels;
        int err = src_process(state, &data);
        long converted = data.output_frames_gen;
        // std::cout << "converted: " << converted << std::endl;
        if (err != 0) continue; // TODO ERROR
        // std::cout << "n8: " << n << std::endl;

        for (size_t i = 0; i < converted; i++) {
            if (mono) {
                // std::cout << "i: " << i << std::endl;
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
    _inverseRatio = static_cast<double>(tempFile.samplerate()) / static_cast<double>(SAMPLE_RATE);
}

void IOMultiFile::clear() {
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();
}

