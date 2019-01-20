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
    if (e->key()==Qt::Key_Escape) {
        // Do not bind!
    } else {
        this->text->updateKey(e->key());
    }

    this->close();
}
