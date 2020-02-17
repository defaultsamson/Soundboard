#include "hotkeyutil.h"

#include <X11/XKBlib.h>
#include <QString>
#include <QHotkey>

QString HotkeyUtil::keycodeToString(quint32 keycode) {
    // Obtain the X11 display.
    Display* display = XOpenDisplay(nullptr);
    if(display == nullptr)
       return QString("ERROR 1");

    return XKeysymToString( XkbKeycodeToKeysym(display, KeyCode(keycode), 0, 0)); // QString::number(keycode);
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(k);
}

