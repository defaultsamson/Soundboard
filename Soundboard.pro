#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T01:11:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Soundboard
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# This uses the -O3 compiler instead of -O2
CONFIG(release, debug|release) {
    CONFIG += optimize_full
}

SOURCES += \
    src/Audio/audioengine.cpp \
    src/Audio/audioobjectfile.cpp \
    src/Audio/audioobjectstream.cpp \
    src/Audio/iomultifile.cpp \
    src/Dialog/dialogboard.cpp \
    src/Dialog/dialogkeybind.cpp \
    src/Dialog/dialogsettings.cpp \
    src/Widget/listitemboard.cpp \
    src/Widget/listitemsound.cpp \
    src/Widget/widgetkeybind.cpp \
    src/Widget/listitem.cpp \
    src/Dialog/dialogsound.cpp \
    src/Widget/widgetaudio.cpp \
    src/mainapp.cpp

HEADERS += \
    src/Audio/audioengine.h \
    src/Audio/audioobject.h \
    src/Audio/audioobjectfile.h \
    src/Audio/audioobjectstream.h \
    src/Audio/iomultifile.h \
    src/Dialog/dialogboard.h \
    src/Dialog/dialogkeybind.h \
    src/Dialog/dialogsettings.h \
    src/Dialog/dialogtestaudio.h \
    src/Widget/listitemboard.h \
    src/Widget/listitemsound.h \
    src/Widget/widgetkeybind.h \
    src/Widget/listitem.h \
    src/Dialog/dialogsound.h \
    src/Widget/widgetaudio.h \
    src/mainapp.h

FORMS += \
    src/Dialog/dialogboard.ui \
    src/Dialog/dialogkeybind.ui \
    src/Dialog/dialogsettings.ui \
    src/Dialog/dialogsound.ui \
    src/mainapp.ui

# Sources for HotkeyPass
HEADERS += src/Hotkey/hotkeypass.h
SOURCES += src/Hotkey/hotkeypass.cpp
mac: SOURCES += src/Hotkey/hotkeypass_mac.cpp
else:win32: SOURCES += src/Hotkey/hotkeypass_win.cpp
else:unix: SOURCES += src/Hotkey/hotkeypass_x11.cpp

# Sources for HotkeyUtil
HEADERS += src/Hotkey/hotkeyutil.h
mac: SOURCES += src/Hotkey/hotkeyutil_mac.cpp
else:win32: SOURCES += src/Hotkey/hotkeyutil_win.cpp
else:unix: SOURCES += src/Hotkey/hotkeyutil_x11.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QDEP_DEPENDS += Skycoder42/QHotkey
!load(qdep):error("Failed to load qdep feature, try running: qdep prfgen --qmake ~/Qt/$${QT_VERSION}/gcc_64/bin/qmake")

# Portaudio
unix:!macx: LIBS += -lportaudio -lrt -lm -lasound -ljack
win32: LIBS += -L$$PWD/../../../../../Qt/5.14.0/mingw73_64/lib/ -lportaudio
win32: INCLUDEPATH += $$PWD/../../../../../Qt/5.14.0/mingw73_64/include
win32: DEPENDPATH += $$PWD/../../../../../Qt/5.14.0/mingw73_64/include

#libsndfile
unix:!macx: LIBS += -lsndfile #-lmpg123 -lmp3lame
win32: LIBS += -L$$PWD/'../../../../../Program Files/Mega-Nerd/libsndfile/lib/' -llibsndfile-1
win32: INCLUDEPATH += $$PWD/'../../../../../Program Files/Mega-Nerd/libsndfile/include'
win32: DEPENDPATH += $$PWD/'../../../../../Program Files/Mega-Nerd/libsndfile/include'

RESOURCES += \
    res.qrc
