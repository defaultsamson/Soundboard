#include "iofile.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <iostream>

IOMultiFile::IOMultiFile()
{

}

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

    // Iterates the open files and adds their read bytes into buffer
    for (int i = _openFiles.size() - 1; i >= 0; i--) {

        sf_count_t read = _openFiles.at(i)->read(readBuffer, static_cast<sf_count_t>(n));
        for (sf_count_t i = 0; i < read; i++) buffer[i] += readBuffer[i];
        if (read > maxRead) maxRead = read;
        if (read < n) delete _openFiles.takeAt(i);
    }

    return static_cast<size_t>(maxRead);
}

void IOMultiFile::openFile(std::string filename) {
    /*
    if (_openFiles.size() > 0) {
        SndfileHandle file = _openFiles.at(0);
        file.seek(0, SEEK_SET);
        return;
    }
    */
    _openFiles.append(new SndfileHandle(filename));
}

void IOMultiFile::clear() {
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
}

