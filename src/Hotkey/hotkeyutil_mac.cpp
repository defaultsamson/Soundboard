#include "hotkeyutil.h"

#include <QString>
#include <QHotkey>

QString HotkeyUtil::keycodeToString(quint32 keycode, bool useNativeTranslation) {
    return "(keycode: " + QString::number(keycode) + ")";
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(k);
}
