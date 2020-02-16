#include "iofile.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <iostream>

IOMultiFile::IOMultiFile()
{

}

void IOMultiFile::write(const float* /*buffer*/, size_t /*n*/) {

}
size_t IOMultiFile::read(float *buffer, size_t n) {
    memset(buffer, 0, n * sizeof(float));
    return mix(buffer, n);
}
size_t IOMultiFile::mix(float *buffer, size_t n) {
    float *readBuffer = new float[n];
    sf_count_t maxRead = 0;

    // Iterates the open files and adds their read bytes into buffer
    for (auto file : _openFiles) {
        sf_count_t read = file.read(readBuffer, static_cast<sf_count_t>(n));
        for (sf_count_t i = 0; i < read; i++) buffer[i] += readBuffer[i];
        if (read > maxRead) maxRead = read;
    }

    delete [] readBuffer;
    return static_cast<size_t>(maxRead);
}

void IOMultiFile::openFile(std::string filename) {
    _openFiles.push_back(SndfileHandle(filename));
}

void IOMultiFile::clear() {
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        _openFiles.takeAt(i);
    }
    _openFiles.clear();
}

