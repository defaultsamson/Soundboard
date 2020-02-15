#ifndef HOTKEYUTIL_H
#define HOTKEYUTIL_H

#include <QString>
#include <QHotkey>

class HotkeyUtil
{
public:
    static QString keycodeToString(quint32 native);
    static QHotkey::NativeShortcut scancodeToNative(quint32 k);
};

#endif // HOTKEYUTIL_H
