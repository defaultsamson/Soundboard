#include "iomultifile.h"

#define SAMPLE_RATE (44100)

IOMultiFile::~IOMultiFile() {
    clear();
    if (_state) src_delete(_state);
}

void IOMultiFile::write(const float* /*buffer*/, size_t /*n*/) {

}
size_t IOMultiFile::read(float *buffer, size_t n) {
    memset(buffer, 0, n * sizeof(float));
    return mix(buffer, n);
}

size_t IOMultiFile::mix(float* buffer, size_t externalN) {

    // Makes it only read what it needs to
    size_t internalN = (_mono ? externalN / 2 : externalN) * _READ_MULTIPLIER;
    float* inBuffer = new float[internalN];

    // Begin with the amount that's available to read
    size_t totalRead = _buffer.availableRead();

    // Iterates the open files and adds their read bytes into buffer
    _modifyLock.lock();
    while (totalRead < externalN && _openFiles.size() > 0) {
        size_t tempTotalRead = 0;
        bool first = true;
        // Iterate the list backwards in case a file needs to be deleted
        for (int i = _openFiles.size() - 1; i >= 0; i--) {

            size_t read = static_cast<size_t>(_openFiles.at(i)->read(inBuffer, static_cast<sf_count_t>(internalN)));

            // Overwrite for the first item iterated
            _buffer.write(inBuffer, read, first, false);
            first = false;

            if (read > tempTotalRead) tempTotalRead = read; // Update the max bytes read
            if (read < internalN) delete _openFiles.takeAt(i); // Remove file from the list if it's run out of bytes to read
        }

        tempTotalRead = _buffer.applySampleRateChange(tempTotalRead, _channels, _state, _data);

        // This will essentially reverse the n /= 2 that we did earlier
        if (_mono) tempTotalRead = _buffer.monoToStereo(tempTotalRead);
        totalRead += tempTotalRead;

        _buffer.forwardWriteIndex(tempTotalRead);
    }
    _modifyLock.unlock();

    delete [] inBuffer;

    return _buffer.read(buffer, totalRead < externalN ? totalRead : externalN, 1.0F, false, true);
}

void IOMultiFile::startRead() {
    _modifyLock.lock();
    _openFiles.append(new SndfileHandle(_filename));
    _modifyLock.unlock();
}

void IOMultiFile::setFile(std::string filename) {
    clear();
    _filename = filename;
    SndfileHandle tempFile(_filename);
    _channels = static_cast<size_t>(tempFile.channels());
    _mono = _channels == 1;
    int error = 0;
    if (_state) src_delete(_state);
    _state = src_new(SRC_SINC_MEDIUM_QUALITY, static_cast<int>(_channels), &error);
    if (error != 0) return; // TODO ERROR
    _data.src_ratio = static_cast<double>(SAMPLE_RATE) / static_cast<double>(tempFile.samplerate());
}

void IOMultiFile::clear() {
    _modifyLock.lock();
    for (int i = _openFiles.size() - 1; i >= 0; i--) {
        delete _openFiles.takeAt(i);
    }
    _modifyLock.unlock();
    _buffer.clear();
}

