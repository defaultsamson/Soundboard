# Soundboard
A Soundboard written in C++ with Qt

## Building
1. Install [Qt 5.12.6+](https://www.qt.io/download) (the free open source version is fine)
2. Clone this repository
3. Read notes below: Install [QHotkey](https://github.com/Skycoder42/QHotkey) using [qdep](https://github.com/Skycoder42/qdep)
    - **DO NOT INSTALL VIA QPMX AS THEY SAY**
    - Note: Really, all you have to do is install qdep and prepare your Qt's qmake version for it (as described in their README). The Soundboard.pro file is already set up properly here, so all you'll have to do is compile the program, and it'll isntall the QHotkey library for you.
4. [Clone PortAudio](http://www.portaudio.com/usinggit.html),  then [install it](http://portaudio.com/docs/v19-doxydocs/tutorial_start.html) (compile it and install) on the system.
    - Note: Be sure to configure PortAudio with `./configure --enable-cxx`
    - Make sure the configuration ends with `ALSA ... yes` and `JACK ... yes`
    - If `JACK ... no` then (on Ubuntu) try `sudo apt install libjack-dev`
5. Build the project! (e.g. Import the project via `Soundboard.pro` into QT Creator and hit the build button

### Linux (AppImage) deployment

6. Install [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy), [linuxdeploy-plugin-qt](https://github.com/linuxdeploy/linuxdeploy-plugin-qt), [linuxdeploy-plugin-appimage](https://github.com/linuxdeploy/linuxdeploy-plugin-appimage), and [AppImageKit](https://github.com/AppImage/AppImageKit), and make sure they're all visible to your environment's `PATH` variable.
    - Note: Personally, I download each one as their .AppImage from the releases page, and I rename all the files from `linuxdeploy-x86_64.AppImage` to `linuxdeploy`, `appimagetool-x86_64.AppImage` to `appimagetool`, etc.
7. Run `./appimage.sh <linuxdeploy executable> <build dir> [optional: Qt's qmake executable]`
    - For example, mine looks like `./appimage.sh linuxdeploy ../build-Soundboard-Desktop_Qt_5_12_5_GCC_64bit-Debug/ ~/Qt/5.12.5/gcc_64/bin/qmake`
    - If the program errors `Failed to run plugin: qt (exit code: 6)`, try running `export QMAKE=/path/to/Qt/qmake` first. For example, I would type `export QMAKE=~/Qt/5.12.5/gcc_64/bin/qmake`
8. If everything went according to plan, it should've exported a Soundboard AppImage to the current working directory

### Windows deployment

Windows deployment is done through the `windows.bat` script. Usage for `windows.bat` is as follows:

`windows.bat <soundboard executable> <windowsdeployqt.exe> [binarycreator.exe] [installerbase.exe]`

Note: the two arguments at the end are only required if you wish to deploy a Windows installer. You can find more about that process [here](https://doc.qt.io/qtinstallerframework/ifw-creating-installers.html) if you'd like to contribute to it. The `windows.bat` script takes care of all that, and the only step you need to do is install a [QT installer framework](https://wiki.qt.io/Qt-Installer-Framework) (look under "Using a prebuilt version") to get the necessary binaries `binarycreator.exe` and `installerbase.exe`.

For example, this is what my command looks like for generating an installer `windows.bat C:\Users\my_username\Documents\GitHub\build-Soundboard-Desktop_Qt_5_12_6_MinGW_64_bit-Release\release\Soundboard.exe C:\Qt\5.12.6\mingw73_64\bin\windeployqt.exe C:\Qt\QtIFW-3.2.0\bin\binarycreator.exe C:\Qt\QtIFW-3.2.0\bin\installerbase.exe`


