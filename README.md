# Soundboard
A Soundboard written in C++ with Qt 5.12.0

## Contributing
1. Install [Qt 5.12.0](https://www.qt.io/download) (the free open source version is fine)
2. Clone this repository
3. Install [QHotkey](https://github.com/Skycoder42/QHotkey)

## Compiling
Download a release of https://github.com/probonopd/linuxdeployqt (in my case I had to build qwertysam's fork)
cd to the Soundboard git repo folder
export PATH=~/Qt/5.12.0/gcc_64/bin:$PATH
~/Documents/GitHub/build-linuxdeployqt-Desktop_Qt_5_12_0_GCC_64bit-Release/bin/linuxdeployqt ~/Documents/GitHub/build-Soundboard-Desktop_Qt_5_12_0_GCC_64bit-Release/Soundboard -unsupported-allow-new-glibc -appimage

Building linuxdeployqt (From qwertysam's fork)
export PATH=~/Qt/5.12.0/gcc_64/bin:$PATH
export LD_LIBRARY_PATH=~/Qt/5.12.0/gcc_64/lib:$LD_LIBRARY
Then build in QT creator
/*
~/Documents/GitHub/build-linuxdeployqt-Desktop_Qt_5_12_0_GCC_64bit-Release/bin/linuxdeployqt ~/Documents/GitHub/build-linuxdeployqt-Desktop_Qt_5_12_0_GCC_64bit-Release/bin/linuxdeployqt -unsupported-allow-new-glibc -appimage
*/
