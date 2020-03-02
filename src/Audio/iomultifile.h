#ifndef IOMULTIFILE_H
#define IOMULTIFILE_H

#include <string>
#include <QList>
#include <sndfile.hh>
#include <samplerate.h>
#include <mutex>
#include "tempbuffer.h"

class IOMultiFile
{
public:
    ~IOMultiFile();

    void write(const float* buffer, size_t n);
    size_t read(float* buffer, size_t n);
    size_t mix(float* buffer, size_t n); // Like readiing, but doesn't overwrite what's already there
    void setFile(std::string filename);
    void clear();
    void startRead();
private:
    SRC_STATE* state;
    SRC_DATA data;
    TempBuffer _buffer;
    double _inverseRatio;
    QList<SndfileHandle*> _openFiles;
    std::mutex modifyLock;
    std::string _filename;
    int _channels = 0;
    bool mono = false;
};

#endif // IOMULTIFILE_H
