#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T01:11:58
#
#-------------------------------------------------

QT       += core gui multimedia

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

# This enables the X11 library to be used. Handles keypresses
# LIBS += -lX11

SOURCES += \
    src/Audio/audioengine.cpp \
    src/Dialog/dialogboard.cpp \
    src/Dialog/dialogkeybind.cpp \
    src/Dialog/dialogsettings.cpp \
    src/Widget/listitemboard.cpp \
    src/Widget/listitemsound.cpp \
    src/Widget/widgetkeybind.cpp \
    src/Widget/listitem.cpp \
    src/Dialog/dialogsound.cpp \
    src/Audio/audiofilestream.cpp \
    src/Widget/widgetaudio.cpp \
    src/Audio/audioobject.cpp \
    src/mainapp.cpp

HEADERS += \
    src/Audio/audioengine.h \
    src/Dialog/dialogboard.h \
    src/Dialog/dialogkeybind.h \
    src/Dialog/dialogsettings.h \
    src/Widget/listitemboard.h \
    src/Widget/listitemsound.h \
    src/Widget/widgetkeybind.h \
    src/Widget/listitem.h \
    src/Dialog/dialogsound.h \
    src/Audio/audiofilestream.h \
    src/Widget/widgetaudio.h \
    src/Audio/audioobject.h \
    src/mainapp.h

FORMS += \
    src/Dialog/dialogboard.ui \
    src/Dialog/dialogkeybind.ui \
    src/Dialog/dialogsettings.ui \
    src/Dialog/dialogsound.ui \
    src/mainapp.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QDEP_DEPENDS += Skycoder42/QHotkey
!load(qdep):error("Failed to load qdep feature")
