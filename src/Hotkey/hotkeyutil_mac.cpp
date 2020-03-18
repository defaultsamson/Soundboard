#include "hotkeyutil.h"

QString HotkeyUtil::keycodeToString(quint32 keycode, bool nonNative) {
    return "(keycode: " + QString::number(keycode) + ")";
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(k);
}
