#ifndef HOTKEYUTIL_H
#define HOTKEYUTIL_H

#include <QString>
#include <QHotkey>
#include <QHash>

class HotkeyUtil
{
public:
    static QString keycodeToString(quint32 native, bool useNativeTranslation = false);
    static QHotkey::NativeShortcut scancodeToNative(quint32 k);
private:
    static bool keymapInited;
    static void initKeymap();
    static QHash<quint32, QString> keymap;
};

#endif // HOTKEYUTIL_H
