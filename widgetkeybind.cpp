#include "widgetkeybind.h"
#include "dialogkeybind.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QWidget>
#include <iostream>

WidgetKeybind::WidgetKeybind(QWidget *parent) :
    QLineEdit(parent),
    _key(-1)
{

}

void WidgetKeybind::setKey(int key) {
    _key = key;
    setText(key == -1 ? "" : QKeySequence(key).toString());
    setPlaceholderText("Click here to set key");
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
