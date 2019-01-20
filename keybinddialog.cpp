#include "keybinddialog.h"
#include "ui_keybinddialog.h"
#include "keybindwidget.h"

#include <QKeyEvent>

KeybindDialog::KeybindDialog(QWidget *parent, KeybindWidget *text) :
    QDialog(parent),
    ui(new Ui::KeybindDialog),
    text(text)
{
    ui->setupUi(this);
}

KeybindDialog::~KeybindDialog()
{
    delete ui;
}

void KeybindDialog::keyPressEvent(QKeyEvent *e) {
    this->text->updateKey(e->key()==Qt::Key_Escape ? -1 : e->key());
    this->close();
}
