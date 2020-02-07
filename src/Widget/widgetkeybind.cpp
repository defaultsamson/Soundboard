#include "widgetkeybind.h"
#include "../Dialog/dialogkeybind.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QWidget>

WidgetKeybind::WidgetKeybind(QWidget *parent) :
    QLineEdit(parent),
    _key(-1),
    _native(-1)
{
    setPlaceholderText("Click here to set key");
}

void WidgetKeybind::setKey(int key, int native) {
    _key = key;
    _native = native;
    setText(key == -1 ? "" : QKeySequence(key).toString());
}

int WidgetKeybind::key() {
    return _key;
}

int WidgetKeybind::nativeKey() {
    return _native;
}

void WidgetKeybind::focusInEvent(QFocusEvent *e) {
    QLineEdit::focusInEvent(e);
    clearFocus();
    DialogKeybind w(parentWidget(), this);
    w.exec();
}
