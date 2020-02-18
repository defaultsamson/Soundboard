# Soundboard
A Soundboard written in C++ with Qt

## Building
1. Install [Qt 5.12.6+](https://www.qt.io/download) (the free open source version is fine)
2. Clone this repository
3. Read notes below: Install [QHotkey](https://github.com/Skycoder42/QHotkey) using [qdep](https://github.com/Skycoder42/qdep)
    - **DO NOT INSTALL VIA QPMX AS THEY SAY**
    - Note: Really, all you have to do is install qdep and prepare your Qt's qmake version for it (as described in their README). The Soundboard.pro file is already set up properly here, so all you'll have to do is compile the program, and it'll isntall the QHotkey library for you.
4. Install [libsndfile](https://github.com/erikd/libsndfile)
    - On Linux `sudo apt install libsndfile-dev`
    - On Windows
        - Download the windows installer from [here](http://www.mega-nerd.com/libsndfile/)
        - Add the `bin` folder to your `PATH` variable (This will make sure it can run from Qt Creator)
5. Install [PortAudio](http://www.portaudio.com/usinggit.html)
    - On Linux `sudo apt install libportaudio2`
    - On Windows: might need to [set up MSYS2 with mingw64](https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2) to compile PortAudio
        - I couldn't get it to compile or install properly no matter what, so what I did is...
        - Installed MSYS2
        - `pacman -S mingw-w64-x86_64-portaudio`
        - Then just copy all the files manually from the msys64 mingw64 installation to out Qt mingw64 installation
        - Copy `C:\msys64\mingw64\bin\libportaudio-2.dll` to my Qt install `C:\Qt\5.14.0\mingw73_64\bin\libportaudio-2.dll`
        - Copy `C:\msys64\mingw64\include\portaudio.h` to my Qt install `C:\Qt\5.14.0\mingw73_64\include\portaudio.h`
        - Copy `C:\msys64\mingw64\lib\libportaudio.a` to my Qt install `C:\Qt\5.14.0\mingw73_64\lib\libportaudio.a`
        - Copy `C:\msys64\mingw64\lib\libportaudio.dll.a` to my Qt install `C:\Qt\5.14.0\mingw73_64\lib\libportaudio.dll.a`
        - Copy `C:\msys64\mingw64\lib\pkgconfig\portaudio-2.0.pc` to my Qt install `C:\Qt\5.14.0\mingw73_64\lib\pkgconfig\portaudio-2.0.pc`
    - Building it yourself
        - Note: Be sure to configure PortAudio with `./configure --enable-cxx`
        - On Linux: make sure the configuration ends with `ALSA ... yes` and `JACK ... yes`
        - If `JACK ... no` then `sudo apt install libjack-dev`
        - [Compile and install](http://portaudio.com/docs/v19-doxydocs/tutorial_start.html) on the system
6. Build the project! (e.g. Import the project via `Soundboard.pro` into QT Creator and hit the build button

### Linux (AppImage) deployment

7. Install [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy), [linuxdeploy-plugin-qt](https://github.com/linuxdeploy/linuxdeploy-plugin-qt), [linuxdeploy-plugin-appimage](https://github.com/linuxdeploy/linuxdeploy-plugin-appimage), and [AppImageKit](https://github.com/AppImage/AppImageKit), and make sure they're all visible to your environment's `PATH` variable.
    - Note: Personally, I download each one as their .AppImage from the releases page, and I rename all the files from `linuxdeploy-x86_64.AppImage` to `linuxdeploy`, `appimagetool-x86_64.AppImage` to `appimagetool`, etc.
8. Run `./appimage.sh <linuxdeploy executable> <build dir> [optional: Qt's qmake executable]`
    - For example, mine looks like `./appimage.sh linuxdeploy ../build-Soundboard-Desktop_Qt_5_12_5_GCC_64bit-Debug/ ~/Qt/5.12.5/gcc_64/bin/qmake`
    - If the program errors `Failed to run plugin: qt (exit code: 6)`, try running `export QMAKE=/path/to/Qt/qmake` first. For example, I would type `export QMAKE=~/Qt/5.12.5/gcc_64/bin/qmake`
8. If everything went according to plan, it should've exported a Soundboard AppImage to the current working directory

### Windows deployment

Windows deployment is done through the `windows.bat` script. Usage for `windows.bat` is as follows:

`windows.bat <soundboard executable> <windowsdeployqt.exe> [binarycreator.exe] [installerbase.exe]`

Note: the two arguments at the end are only required if you wish to deploy a Windows installer. You can find more about that process [here](https://doc.qt.io/qtinstallerframework/ifw-creating-installers.html) if you'd like to contribute to it. The `windows.bat` script takes care of all that, and the only step you need to do is install a [QT installer framework](https://wiki.qt.io/Qt-Installer-Framework) (look under "Using a prebuilt version") to get the necessary binaries `binarycreator.exe` and `installerbase.exe`.

For example, this is what my command looks like for generating an installer `windows.bat C:\Users\my_username\Documents\GitHub\build-Soundboard-Desktop_Qt_5_12_6_MinGW_64_bit-Release\release\Soundboard.exe C:\Qt\5.12.6\mingw73_64\bin\windeployqt.exe C:\Qt\QtIFW-3.2.0\bin\binarycreator.exe C:\Qt\QtIFW-3.2.0\bin\installerbase.exe`

NOTE: The 5.14.0 MingW64 `windeployqt.exe` is bugged. You just need to get an un-bugged file, which can be found in [this thread](https://forum.qt.io/topic/109779/windeployqt-exe-comes-with-qt-5-14-not-copy-the-dlls-to-the-app-directory)

Also NOTE: When building, may need to initially say "no" when the script asks if the program didn't run okay, then copy `libportaudio-2.dll` and `libsndfile-1.dll` into `Soundboard/lib`, then continue.


