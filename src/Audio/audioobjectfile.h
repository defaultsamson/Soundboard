#ifndef AUDIOOBJECTFILE_H
#define AUDIOOBJECTFILE_H

#include <QString>

#include "audioobject.h"
#include "iomultifile.h"
#include <portaudio.h>
#include <sndfile.hh>
#include <string>
#include <mutex>

class AudioObjectFile : public AudioObject
{
public:
    ~AudioObjectFile();
    void setFile(const QString &filePath);

    void write(const float* buffer, size_t n);
    size_t read(float* buffer, size_t n);

    void play();
    void stop();
    void pause();

    bool isPlaying();
    bool isPaused();

    bool hasFile();

    bool doMix();

private:
    IOMultiFile _file;
    std::string _filename;
    bool _hasFile = false;
    bool paused = false;
};

#endif // AUDIOOBJECTFILE_H
