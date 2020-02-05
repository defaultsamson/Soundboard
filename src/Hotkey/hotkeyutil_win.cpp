#include "hotkeyutil.h"

#include <QString>
#include <QHotkey>
#include <qt_windows.h>

QString HotkeyUtil::keycodeToString(quint32 keycode) {
    return QString::number(keycode);
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(MapVirtualKeyA(k, MAPVK_VSC_TO_VK));
}
