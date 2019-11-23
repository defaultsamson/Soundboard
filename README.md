# Soundboard
A Soundboard written in C++ with Qt

## Building
1. Install [Qt 5.12.5](https://www.qt.io/download) (the free open source version is fine)
2. Clone this repository
3. Read notes below: Install [QHotkey](https://github.com/Skycoder42/QHotkey) using [qdep](https://github.com/Skycoder42/qdep)
    - **DO NOT INSTALL VIA QPMX AS THEY SAY**
    - Note: Really, all you have to do is install qdep and prepare your Qt's qmake version for it (as described in their README). The Soundboard.pro file is already set up properly here, so all you'll have to do is compile the program, and it'll isntall the QHotkey library for you.
4. Build the project! (e.g. Import the project via `Soundboard.pro` into QT Creator and hit the build button

If you'd like to compile it to an AppImage

5. Install [linuxdeploy](https://github.com/linuxdeploy/linuxdeploy), [linuxdeploy-plugin-qt](https://github.com/linuxdeploy/linuxdeploy-plugin-qt), [linuxdeploy-plugin-appimage](https://github.com/linuxdeploy/linuxdeploy-plugin-appimage), and [AppImageKit](https://github.com/AppImage/AppImageKit), and make sure they're all visible to your environment's `PATH` variable.
    - Note: Personally, I download each one as their .AppImage from the releases page, and I rename all the files from `linuxdeploy-x86_64.AppImage` to `linuxdeploy`, `appimagetool-x86_64.AppImage` to `appimagetool`, etc.
6. Run `./appimage-ify.sh <linux deploy executable> <build dir> [optional: Qt's qmake executable]`
    - For example, mine looks like `./appimage-ify.sh linuxdeploy ../build-Soundboard-Desktop_Qt_5_12_5_GCC_64bit-Debug/ ~/Qt/5.12.5/gcc_64/bin/qmake`
    - If the program errors "Failed to run plugin: qt (exit code: 6)", try running `export QMAKE=/path/to/Qt/qmake` first. For example, mine is `export QMAKE=~/Qt/5.12.5/gcc_64/bin/qmake`
7. If everything wend according to plan, it should've exported a Soundboard AppImage to the current directory
