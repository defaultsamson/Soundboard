#include "iomultifile.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <samplerate.h>
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
    //n = _inverseRatio * n;
    //n += (n % 2); // if the number isn't even, make it even
    // std::cout << "n: " << n << std::endl;


    // Makes it only read what it needs to
    if (mono) n /= 2;
    float* inBuffer = new float[n];
    float* outBuffer = new float[n];
    size_t totalRead = 0;

    data.data_in = inBuffer;
    data.data_out = outBuffer;

    // Iterates the open files and adds their read bytes into buffer
    modifyLock.lock();
    while (totalRead < n && _openFiles.size() > 0) {
        size_t tempTotalRead = 0;
        bool first = true;
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

            // Overwrite for the first item iterated
            _buffer.write(inBuffer, n, first, false);
            first = false;

            if (read > tempTotalRead) tempTotalRead = read; // Update the max bytes read
            if (read < n) delete _openFiles.takeAt(i); // Remove file from the list if it's run out of bytes to read
        }
        if (mono) {
            // std::cout << "1" << std::endl;
            tempTotalRead = _buffer.monoToStereo(tempTotalRead);
            // std::cout << "2" << std::endl;
        }
        _buffer.forwardWriteIndex(tempTotalRead);
        totalRead += tempTotalRead;
    }
    modifyLock.unlock();

    // if (mono) totalRead = _buffer.monoToStereo(tempTotalRead);
    // totalRead = mono ? totalRead * 2 : totalRead;

    delete [] inBuffer;
    delete [] outBuffer;

    return _buffer.read(buffer, totalRead, 1.0F, false);
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
    _buffer.forwardWriteIndex(280);
    _buffer.forwardReadIndex(280);
}

