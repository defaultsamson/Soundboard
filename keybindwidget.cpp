#include "keybindwidget.h"
#include "keybinddialog.h"

#include <QKeySequence>

KeybindWidget::KeybindWidget(QWidget *parent) :
    QLineEdit(parent),
    key(-1)
{

}

void KeybindWidget::focusInEvent(QFocusEvent *) {
    this->clearFocus();
    KeybindDialog w(this->parentWidget(), this);
    w.exec();
}

void KeybindWidget::updateKey(int key) {
    this->key = key;
    this->setText(key == -1 ? "" : QKeySequence(key).toString());
}
