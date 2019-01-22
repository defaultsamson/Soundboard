#include "keybindwidget.h"
#include "dialogkeybind.h"

#include <QKeySequence>
#include <iostream>

KeybindWidget::KeybindWidget(QWidget *parent) :
    QLineEdit(parent),
    key(-1)
{

}

void KeybindWidget::focusInEvent(QFocusEvent *) {
    this->clearFocus();
    DialogKeybind w(this->parentWidget(), this);
    w.exec();
}

void KeybindWidget::updateKey(int key) {
    this->key = key;
    this->setText(key == -1 ? "" : QKeySequence(key).toString());
}
