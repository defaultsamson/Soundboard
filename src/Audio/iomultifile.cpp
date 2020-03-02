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
    //n = _inverseRatio * n;
    //n += (n % 2); // if the number isn't even, make it even
    // std::cout << "n: " << n << std::endl;

    float inBuffer[n];
    float outBuffer[n];
    size_t maxRead = 0;

    // Makes it only read what it needs to
    if (mono) n /= 2;

    //data.data_in = inBuffer;
    //data.data_out = outBuffer;

    // Iterates the open files and adds their read bytes into buffer
    modifyLock.lock();
    // Iterate the list backwards in case a file needs to be deleted
    for (int i = _openFiles.size() - 1; i >= 0; i--) {

        size_t read = static_cast<size_t>(_openFiles.at(i)->read(inBuffer, static_cast<sf_count_t>(n)));

        /*
        data.input_frames = read / _channels;
        data.output_frames = n / _channels;
        int err = src_process(state, &data);
        long converted = data.output_frames_gen;
        if (err != 0) continue; // TODO ERROR
        */

        for (size_t i = 0; i < n; i++) {
            if (mono) {
                buffer[(i * 2)]     += inBuffer[i];
                buffer[(i * 2) + 1] += inBuffer[i];
            }
            else buffer[i] += inBuffer[i];
        }

        if (read > maxRead) maxRead = read; // Update the max bytes read
        if (read < n) delete _openFiles.takeAt(i); // Remove file from the list if it's run out of bytes to read
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
    //state = src_new(SRC_SINC_MEDIUM_QUALITY, _channels, &error);
    if (error != 0) return; // TODO ERROR
    //data.src_ratio = static_cast<double>(SAMPLE_RATE) / static_cast<double>(tempFile.samplerate());
    //_inverseRatio = static_cast<double>(tempFile.samplerate()) / static_cast<double>(SAMPLE_RATE);
}

void IOMultiFile::clear() {
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();
    _buffer.clear();
}

