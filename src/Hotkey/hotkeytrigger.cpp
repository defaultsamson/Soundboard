#include "hotkeytrigger.h"

#include <QObject>
#include <QHotkey>
#include "../mainapp.h"
#include "../Hotkey/hotkeypass.h"
#include "../Hotkey/hotkeyutil.h"

HotkeyTrigger::HotkeyTrigger(Main* _main, std::function<void()> trigger)
    : QObject(),
      _main(_main),
      _hasKey(false),
      _key(0),
      _hotkey(new HotkeyPass(_main)),
      _trigger(trigger)
{
    // Sets up the keybind
    QObject::connect(_hotkey, &QHotkey::activated, this, trigger);
}

HotkeyTrigger::~HotkeyTrigger() {
    delete _hotkey;
}

void HotkeyTrigger::setKey(quint32 k) {
    _key = k;
    _hasKey = true;
    _hotkey->setNativeShortcut(HotkeyUtil::scancodeToNative(k));
}
