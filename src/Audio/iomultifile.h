#ifndef IOMULTIFILE_H
#define IOMULTIFILE_H

#include <string>
#include <QList>
#include <sndfile.hh>
#include <mutex>

class IOMultiFile
{
public:
    ~IOMultiFile();

    void write(const float* buffer, size_t n);
    size_t read(float* buffer, size_t n);
    size_t mix(float* buffer, size_t n); // Like readiing, but doesn't overwrite what's already there
    void openFile(std::string filename);
    void clear();
private:
    QList<SndfileHandle*> _openFiles;
    std::mutex modifyLock;
};

#endif // IOMULTIFILE_H
