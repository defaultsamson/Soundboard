#include "hotkeyutil.h"

#include <X11/XKBlib.h>

bool HotkeyUtil::keymapInited = false;
QHash<quint32, QString> HotkeyUtil::keymap = QHash<quint32, QString>();

QString HotkeyUtil::keycodeToString(quint32 keycode, bool nonNative) {
    // Prioritize non-native translations if told to
    if (nonNative) {
        if (!keymapInited) initKeymap();
        QString name = keymap.value(keycode);
        if (!name.isNull() && !name.isEmpty()) return name;
    }
    // If the program got here, no non-native translations were found. So fallback
    // to the system callback.

    // Obtain the X11 display.
    Display* display = XOpenDisplay(nullptr);
    if(display == nullptr)
       return "ERROR 0 (keycode: " + QString::number(keycode) + ")";

    char* name = XKeysymToString( XkbKeycodeToKeysym(display, KeyCode(keycode), 0, 0));

    XFlush(display);
    XCloseDisplay(display);

    return name;
}

QHotkey::NativeShortcut HotkeyUtil::scancodeToNative(quint32 k) {
    return QHotkey::NativeShortcut(k);
}

void HotkeyUtil::initKeymap() {
    keymapInited = true;
    keymap.insert(9, "Escape");
    keymap.insert(67, "F1");
    keymap.insert(68, "F2");
    keymap.insert(69, "F3");
    keymap.insert(70, "F4");
    keymap.insert(71, "F5");
    keymap.insert(72, "F6");
    keymap.insert(73, "F7");
    keymap.insert(74, "F8");
    keymap.insert(75, "F9");
    keymap.insert(76, "F10");
    keymap.insert(95, "F11");
    keymap.insert(96, "F12");

    keymap.insert(49, "~");
    keymap.insert(10, "1");
    keymap.insert(11, "2");
    keymap.insert(12, "3");
    keymap.insert(13, "4");
    keymap.insert(14, "5");
    keymap.insert(15, "6");
    keymap.insert(16, "7");
    keymap.insert(17, "8");
    keymap.insert(18, "9");
    keymap.insert(19, "0");
    keymap.insert(20, "-");
    keymap.insert(21, "=");
    keymap.insert(22, "Backspace");

    keymap.insert(23, "Tab");
    keymap.insert(24, "Q");
    keymap.insert(25, "W");
    keymap.insert(26, "E");
    keymap.insert(27, "R");
    keymap.insert(28, "T");
    keymap.insert(29, "Y");
    keymap.insert(30, "U");
    keymap.insert(31, "I");
    keymap.insert(32, "O");
    keymap.insert(33, "P");
    keymap.insert(34, "[");
    keymap.insert(35, "]");
    keymap.insert(51, "\\");

    keymap.insert(66, "Caps Lock");
    keymap.insert(38, "A");
    keymap.insert(39, "S");
    keymap.insert(40, "D");
    keymap.insert(41, "F");
    keymap.insert(42, "G");
    keymap.insert(43, "H");
    keymap.insert(44, "J");
    keymap.insert(45, "K");
    keymap.insert(46, "L");
    keymap.insert(47, ";");
    keymap.insert(48, "'");
    keymap.insert(36, "Enter");

    keymap.insert(50, "Left Shift");
    keymap.insert(52, "Z");
    keymap.insert(53, "X");
    keymap.insert(54, "C");
    keymap.insert(55, "V");
    keymap.insert(56, "B");
    keymap.insert(57, "N");
    keymap.insert(58, "M");
    keymap.insert(59, ",");
    keymap.insert(60, ".");
    keymap.insert(61, "/");
    keymap.insert(62, "Right Shift");

    keymap.insert(37, "Left Control");
    keymap.insert(133, "Left Start");
    keymap.insert(64, "Left Alt");
    keymap.insert(65, "Space");
    keymap.insert(108, "Right Alt");
    keymap.insert(134, "Right Start");
    keymap.insert(135, "Menu");
    keymap.insert(105, "Right Control");

    keymap.insert(78, "Scroll Lock");
    keymap.insert(127, "Pause");

    keymap.insert(118, "Insert");
    keymap.insert(110, "Home");
    keymap.insert(112, "Page Up");

    keymap.insert(119, "Delete");
    keymap.insert(115, "End");
    keymap.insert(117, "Page Down");

    keymap.insert(111, "Up");
    keymap.insert(113, "Left");
    keymap.insert(116, "Down");
    keymap.insert(114, "Right");

    keymap.insert(77, "Num Lock");
    keymap.insert(106, "Numpad /");
    keymap.insert(63, "Numpad *");
    keymap.insert(82, "Numpad -");

    keymap.insert(79, "Numpad 7");
    keymap.insert(80, "Numpad 8");
    keymap.insert(81, "Numpad 9");
    keymap.insert(86, "Numpad +");

    keymap.insert(83, "Numpad 4");
    keymap.insert(84, "Numpad 5");
    keymap.insert(85, "Numpad 6");

    keymap.insert(87, "Numpad 1");
    keymap.insert(88, "Numpad 2");
    keymap.insert(89, "Numpad 3");
    keymap.insert(104, "Numpad Enter");

    keymap.insert(90, "Numpad 0");
    keymap.insert(91, "Numpad .");

    // This is only the naming scheme when numlock is on
    /*
    keymap.insert(79, "Numpad Home");
    keymap.insert(80, "Numpad Up");
    keymap.insert(81, "Numpad Page Up");
    keymap.insert(83, "Numpad Left");
    keymap.insert(84, "Numpad Middle");
    keymap.insert(85, "Numpad Right");
    keymap.insert(87, "Numpad End");
    keymap.insert(88, "Numpad Down");
    keymap.insert(89, "Numpad Page Down");
    keymap.insert(90, "Numpad Insert");
    keymap.insert(91, "Numpad Delete");
    */
}
