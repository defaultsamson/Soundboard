#include "widgetkeybind.h"
#include "../Dialog/dialogkeybind.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QWidget>

WidgetKeybind::WidgetKeybind(QWidget *parent) :
    QLineEdit(parent),
    _key(-1)
{
    setPlaceholderText("Click here to set key");
}

void WidgetKeybind::setKey(int key) {
    _key = key;
    setText(key == -1 ? "" : QKeySequence(key).toString());
}

int WidgetKeybind::key() {
    return _key;
}

void WidgetKeybind::focusInEvent(QFocusEvent *e) {
    QLineEdit::focusInEvent(e);
    clearFocus();
    DialogKeybind w(parentWidget(), this);
    w.exec();
}
