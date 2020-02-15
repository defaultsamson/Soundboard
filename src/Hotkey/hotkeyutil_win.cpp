#include "hotkeyutil.h"

#include <QString>
#include <QHotkey>
#include <qt_windows.h>

QString HotkeyUtil::keycodeToString(quint32 keycode) {
    // https://stackoverflow.com/questions/38100667/windows-virtual-key-codes
    int size = 1024;
    char* name = static_cast<char*>(malloc(size * sizeof(char)));
    long lParamValue = keycode << 16;
    GetKeyNameTextA(lParamValue, name, size);
    QString ret = QString(name);
    free(name);
    return ret;
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(MapVirtualKeyA(k, MAPVK_VSC_TO_VK));
}
