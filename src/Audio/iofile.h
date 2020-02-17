#ifndef IOFILE_H
#define IOFILE_H

#include "iodevice.h"
#include <string>
#include <QList>
#include <sndfile.hh>

class IOMultiFile : public IODevice
{
public:
    IOMultiFile();
    ~IOMultiFile();

    void write(const float *buffer, size_t n);
    size_t read(float *buffer, size_t n);
    size_t mix(float *buffer, size_t n);
    void openFile(std::string filename);
    void clear();
private:
    QList<SndfileHandle*> _openFiles;
};

#endif // IOFILE_H
