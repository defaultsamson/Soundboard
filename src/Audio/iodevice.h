#ifndef IODEVICE_H
#define IODEVICE_H

#include <stddef.h>

class IODevice
{
public:
    virtual ~IODevice() {}
    virtual void write(const float *buffer, size_t n) = 0;
    virtual size_t read(float *buffer, size_t n) = 0;
    virtual size_t mix(float *buffer, size_t n) = 0; // Like readiing, but doesn't overwrite what's already there
};

#endif // IODEVICE_H
