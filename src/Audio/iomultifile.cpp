#include "iomultifile.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <iostream>

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
    float readBuffer[n];
    sf_count_t maxRead = 0;

    // Makes it only read what it needs to
    if (mono) n /= 2;

    // Iterates the open files and adds their read bytes into buffer
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {

        sf_count_t read = _openFiles.at(i)->read(readBuffer, static_cast<sf_count_t>(n));
        for (sf_count_t i = 0; i < read; i++) {
            if (mono) {
                buffer[(i * 2)] += readBuffer[i];
                buffer[(i * 2) + 1] += readBuffer[i];
            }
            else buffer[i] += readBuffer[i];
        }
        if (read > maxRead) maxRead = read;
        if (read < n) delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();

    return static_cast<size_t>(mono ? maxRead * 2 : maxRead);
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
}

void IOMultiFile::clear() {
    modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
    modifyLock.unlock();
}

