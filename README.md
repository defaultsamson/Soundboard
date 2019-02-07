# Soundboard
A Soundboard written in C++ with Qt 5.12.0

## Contributing
1. Install a version of [Qt](https://www.qt.io/download) (the free open source version is fine)
2. Add the following to your `~/.profile`.
```
PATH=$HOME/Qt/<version>/gcc_64/bin:$PATH # Gives access to the desired qmake binary
LD_LIBRARY_PATH=$HOME/Qt/<version>/gcc_64/lib:$LD_LIBRARY # Gives access to Qt libraries (for compiling)
```
3. Clone this repository 

You can update the internal [QHotkey](https://github.com/Skycoder42/QHotkey) library if you so desire. Make sure to copy the files from the `QHotkey` folder (not the examples folder) and add the script from `qhotkey.pri` and `qhotkey.prc` into `Soundboard.pro` near the bottom.

## Compiling on Linux
### For Linux
Use a release of the [linuxdeployqt tool](https://github.com/qwertysam/linuxdeployqt).

1. Build the release version of the qt project (not debug).
2. cd to the release folder.
3. `/path/to/linuxdeployqt.AppImage ./Soundboard -allow-new-glibc -appimage`

### For Windows
Follow [this guide](https://stackoverflow.com/questions/14170590/building-qt-5-on-linux-for-windows/14170591#14170591) for setting up MXE with mingw to compile qt (use `make qt5`, NOT `make qtbase`, and remember to `export PATH=<mxe root>/usr/bin:$PATH`).

### For Mac
Contributions would be greatly appreciated!
The macdeployqt tool may be a good place to start.

## Compiling on Windows
Contributions would be greatly appreciated!

## Compiling on Mac
Contributions would be greatly appreciated!


## Compiling linuxdeployqt
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
