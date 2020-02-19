#include "widgetkeybind.h"
#include "../Dialog/dialogkeybind.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QWidget>
#include <QString>
#include "../Hotkey/hotkeyutil.h"

WidgetKeybind::WidgetKeybind(QWidget* parent) :
    QLineEdit(parent),
    _key(0),
    _hasKey(false)
{
    setPlaceholderText("Click here to set key");
}

void WidgetKeybind::setKey(quint32 key) {
    _key = key;
    _hasKey = true;
    setText(HotkeyUtil::keycodeToString(_key));
}

void WidgetKeybind::unSetKey() {
    _key = 0;
    _hasKey = false;
    setText("");
}

quint32 WidgetKeybind::key() {
    return _key;
}

bool WidgetKeybind::hasKey() {
    return _hasKey;
}

void WidgetKeybind::focusInEvent(QFocusEvent* e) {
    QLineEdit::focusInEvent(e);
    clearFocus();
    DialogKeybind w(parentWidget(), this);
    w.exec();
}
