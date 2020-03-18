#ifndef IOMULTIFILE_H
#define IOMULTIFILE_H

#include "tempbuffer.h"

#include <string>
#include <QList>
#include <sndfile.hh>
#include <samplerate.h>
#include <mutex>

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
    SRC_STATE* _state = nullptr;
    SRC_DATA _data;
    TempBuffer _buffer;
    QList<SndfileHandle*> _openFiles;
    std::mutex _modifyLock;
    std::string _filename;
    size_t _channels = 0;
    bool _mono = false;

    // This makes the files need to read _READ_MULTIPLIER times fewer (it may need to read more anyways if there's samplerate changes to be made)
    size_t _READ_MULTIPLIER = 8;
};

#endif // IOMULTIFILE_H
