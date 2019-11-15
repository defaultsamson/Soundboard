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
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        src/mainwindow.cpp \
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
    src/Audio/audioobject.cpp

HEADERS += \
        src/mainwindow.h \
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
    src/Audio/audioobject.h

FORMS += \
        src/mainwindow.ui \
    src/Dialog/dialogboard.ui \
    src/Dialog/dialogkeybind.ui \
    src/Dialog/dialogsettings.ui \
    src/Dialog/dialogsound.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

!ReleaseBuild:!DebugBuild:!system(qpmx -d $$shell_quote($$_PRO_FILE_PWD_) --qmake-run init $$QPMX_EXTRA_OPTIONS $$shell_quote($$QMAKE_QMAKE) $$shell_quote($$OUT_PWD)): error(qpmx initialization failed. Check the compilation log for details.)
else: include($$OUT_PWD/qpmx_generated.pri)
