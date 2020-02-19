#include "hotkeypass.h"

#include <QObject>
#include <QHotkey>

HotkeyPass::HotkeyPass(QObject* parent) :
    QHotkey(parent)
{
    QObject::connect(this, &QHotkey::activated, this, &HotkeyPass::triggerPassthrough);
}
