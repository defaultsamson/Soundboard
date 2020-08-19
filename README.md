# Soundboard ![Linux](https://github.com/qwertysam/Soundboard/workflows/Linux/badge.svg) ![Windows](https://github.com/qwertysam/Soundboard/workflows/Windows/badge.svg)
A Soundboard written in C++

## Technologies
- [Qt](https://www.qt.io/) (front-end)
- [PortAudio](http://www.portaudio.com/) (audio device back-end)
- [libsndfile](http://www.mega-nerd.com/libsndfile/) (audio decoding)
- [libsamplerate](http://www.mega-nerd.com/SRC/) (samplerate conversion)
- [QHotkey](https://github.com/Skycoder42/QHotkey) (hotkey back-end)
- [SingleApplication](https://github.com/itay-grudev/SingleApplication) (application manager)

## Building
1. Install [Qt 5.12.6+](https://www.qt.io/download) (the free open source version is fine)
2. Clone this repository `git clone https://github.com/qwertysam/Soundboard`
3. Install [qdep](https://github.com/Skycoder42/qdep)
    - Refreshing the Soundboard.pro project in Qt Creator should automatically install the [QHotkey](https://github.com/Skycoder42/QHotkey) and [SingleApplication](https://github.com/itay-grudev/SingleApplication) dependencies
4. Install [libsndfile](https://github.com/erikd/libsndfile)
    - Without MP3 support:
        - On Linux `sudo apt install libsndfile-dev`
        - On Windows
            - Download the windows installer from [here](http://www.mega-nerd.com/libsndfile/)
            - Add the `bin` folder to your `PATH` variable (This will make sure it can run from Qt Creator)
    - <details>
          <summary>With MP3 support: (THIS IS A WIP)</summary>
    
        - On Linux
            - For Linux (MAYBE BROKEN???):
                - `git clone --single-branch --branch mpeg-support https://github.com/arthurt/libsndfile`
                    - Until the main libsndfile repo gets mp3 support, clone [arthurt's fork](https://github.com/arthurt/libsndfile) & the `mpeg-support`
                - `./autogen.sh`
                - `./configure --enable-experimental --enable-mpeg`
                    - According to the [MPEG pull request](https://github.com/erikd/libsndfile/pull/499) on the main repo, you need the `--enable-experimental` tag
                - `make`
                - `sudo make install`
            - For Windows (BROKEN!!):
                - Install 64-bit MingW with `sudo apt install mingw-w64`
                - `git clone --single-branch --branch mpeg-support https://github.com/arthurt/libsndfile`
                    - Until the main libsndfile repo gets mp3 support, clone [arthurt's fork](https://github.com/arthurt/libsndfile) & the `mpeg-support`
                - `./autogen.sh`
                - `./configure --enable-experimental --enable-mpeg --host=x86_64-w64-mingw32`
                    - According to the [MPEG pull request](https://github.com/erikd/libsndfile/pull/499) on the main repo, you need the `--enable-experimental` tag
                - `make`
                - Then copy `./libsndfile/src/.libs/libsndfile-1.dll` to the Qt installation `bin` folder `C:\Qt\VERSION\mingw73_64\bin\`
                - Then copy `./libsndfile/src/.libs/libsndfile.dll.a` to the Qt installation `lib` folder `C:\Qt\VERSION\mingw73_64\lib\`
        - On Windows
            - Good luck?
    
        </details>

5. Install [libsamplerate](https://github.com/erikd/libsamplerate)
    - On Linux `sudo apt install libsamplerate-dev`
    - On Windows: 
        - I couldn't find pre-compiled binaries for windows, so what I did was open a Linux terminal and...
        - `sudo apt install mingw-w64`
        - `git clone https://github.com/erikd/libsamplerate`
        - `cd libsamplerate`
        - `./autogen.sh`
        - `./configure --host=x86_64-w64-mingw32.shared`
        - `make`
        - Then just copy all the files manually from the libsamplerate build to our Windows Qt mingw64 installation
          ```
          ./src/.libs/libsamplerate.a     ->  C:\Qt\VERSION\mingw73_64\lib\libsamplerate.a
          ./src/.libs/libsamplerate.dll.a ->  C:\Qt\VERSION\mingw73_64\lib\libsamplerate.dll.a
          ./src/.libs/libsamplerate-0.dll ->  C:\Qt\VERSION\mingw73_64\bin\libsamplerate-0.dll
          ./src/samplerate.h              ->  C:\Qt\VERSION\mingw73_64\include\samplerate.h
          ./samplerate.pc                 ->  C:\Qt\VERSION\mingw73_64\lib\pkgconfig\samplerate.pc
          ```
    - Building it yourself
        - Note: Be sure to configure PortAudio with `./configure --enable-cxx`
        - [Compile and install](http://portaudio.com/docs/v19-doxydocs/tutorial_start.html) on the system
6. Install [PortAudio](http://www.portaudio.com/usinggit.html)
    - On Linux `sudo apt install libportaudio2`
    - On Windows: might need to [set up MSYS2 with mingw64](https://github.com/orlp/dev-on-windows/wiki/Installing-GCC--&-MSYS2) to compile PortAudio
        - I couldn't get it to compile or install properly no matter what, so what I did is...
        - Installed MSYS2
        - `pacman -S mingw-w64-x86_64-portaudio`
        - Then just copy all the files manually from the msys64 mingw64 installation to our Windows Qt mingw64 installation
          ```
          C:\msys64\mingw64\lib\libportaudio.a         ->  C:\Qt\VERSION\mingw73_64\lib\libportaudio.a
          C:\msys64\mingw64\lib\libportaudio.dll.a     ->  C:\Qt\VERSION\mingw73_64\lib\libportaudio.dll.a
          C:\msys64\mingw64\bin\libportaudio-2.dll     ->  C:\Qt\VERSION\mingw73_64\bin\libportaudio-2.dll
          C:\msys64\mingw64\include\portaudio.h        ->  C:\Qt\VERSION\mingw73_64\include\portaudio.h
          C:\msys64\mingw64\pkgconfig\portaudio-2.0.pc ->  C:\Qt\VERSION\mingw73_64\lib\pkgconfig\portaudio-2.0.pc
          ```
    - Building it yourself
        - Note: Be sure to configure PortAudio with `./configure --enable-cxx`
            - On Linux: make sure the configuration ends with `ALSA ... yes` and `JACK ... yes`
                - If `JACK ... no` then `sudo apt install libjack-dev`
        - [Compile and install](http://portaudio.com/docs/v19-doxydocs/tutorial_start.html) on the system
7. Build the project! (e.g. Import the project via `Soundboard.pro` into QT Creator and hit the build button

### Linux (AppImage) deployment

8. Install [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy), [linuxdeploy-plugin-qt](https://github.com/linuxdeploy/linuxdeploy-plugin-qt), [linuxdeploy-plugin-appimage](https://github.com/linuxdeploy/linuxdeploy-plugin-appimage), and [AppImageKit](https://github.com/AppImage/AppImageKit), and make sure they're all visible to your environment's `PATH` variable.
    - Note: Personally, I download each one as their .AppImage from the releases page, and I rename all the files from `linuxdeploy-x86_64.AppImage` to `linuxdeploy`, `appimagetool-x86_64.AppImage` to `appimagetool`, etc.
9. Run `./appimage.sh <linuxdeploy executable> <build dir> [optional: Qt's qmake executable]`
    - For example, mine looks like `./appimage.sh linuxdeploy ../build-Soundboard-Desktop_Qt_5_12_5_GCC_64bit-Debug/ ~/Qt/5.12.5/gcc_64/bin/qmake`
    - If the program errors `Failed to run plugin: qt (exit code: 6)`, try running `export QMAKE=/path/to/Qt/qmake` first. For example, I would type `export QMAKE=~/Qt/5.12.5/gcc_64/bin/qmake`
10. If everything went according to plan, it should've exported a Soundboard AppImage to the current working directory

### Windows deployment

Windows deployment is done through the `windows.bat` script. Usage for `windows.bat` is as follows:

`windows.bat <soundboard executable> <windeployqt.exe> [binarycreator.exe] [installerbase.exe]`

Note: the two arguments at the end are only required if you wish to deploy a Windows installer. You can find more about that process [here](https://doc.qt.io/qtinstallerframework/ifw-creating-installers.html) if you'd like to contribute to it. The `windows.bat` script takes care of all that, and the only step you need to do is install a [QT installer framework](https://wiki.qt.io/Qt-Installer-Framework) (look under "Using a prebuilt version") to get the necessary binaries `binarycreator.exe` and `installerbase.exe`.

For example, this is what my command looks like for generating an installer `windows.bat C:\Users\my_username\Documents\GitHub\build-Soundboard-Desktop_Qt_5_12_6_MinGW_64_bit-Release\release\Soundboard.exe C:\Qt\5.12.6\mingw73_64\bin\windeployqt.exe C:\Qt\QtIFW-3.2.0\bin\binarycreator.exe C:\Qt\QtIFW-3.2.0\bin\installerbase.exe`

NOTE: The 5.14.0 MingW64 `windeployqt.exe` is bugged. You just need to get an un-bugged file, which can be found in [this thread](https://forum.qt.io/topic/109779/windeployqt-exe-comes-with-qt-5-14-not-copy-the-dlls-to-the-app-directory)

Also NOTE: When building, may need to initially say "no" when the script asks if the program didn't run okay, then copy `libportaudio-2.dll`, `libsamplerate-0.dll`, and `libsndfile-1.dll` into `Soundboard/lib`, then continue.

## Common Compile Errors (Linux)
- `error: 'portaudio.h' file not found`
    - Solution: `sudo apt install libasound-dev portaudio19-dev`
- `error: X11/XKBlib.h: No such file or directory`
    - Solution: `sudo apt install libx11-dev`
- `error: cannot find -lGL`
    - Solution: `sudo apt install libgl1-mesa-dev`
