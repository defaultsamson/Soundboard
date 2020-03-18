#include "hotkeypass.h"

#include <QObject>

HotkeyPass::HotkeyPass(QObject* parent) :
    QHotkey(parent)
{
    QObject::connect(this, &QHotkey::activated, this, &HotkeyPass::triggerPassthrough);
}
