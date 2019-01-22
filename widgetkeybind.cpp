#include "widgetkeybind.h"
#include "dialogkeybind.h"

#include <QKeySequence>
#include <QLineEdit>
#include <QWidget>
#include <iostream>

WidgetKeybind::WidgetKeybind(QWidget *parent) :
    QLineEdit(parent),
    key(-1)
{

}

void WidgetKeybind::focusInEvent(QFocusEvent *e) {
    QLineEdit::focusInEvent(e);
    this->clearFocus();
    DialogKeybind w(this->parentWidget(), this);
    w.exec();
}

void WidgetKeybind::updateKey(int key) {
    this->key = key;
    this->setText(key == -1 ? "" : QKeySequence(key).toString());
}
