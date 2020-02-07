#include "hotkeyutil.h"

#include <QString>

QString HotkeyUtil::keycodeToString(quint32 keycode) {
    return QString::number(keycode);
}
