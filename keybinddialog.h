#ifndef KEYBINDDIALOG_H
#define KEYBINDDIALOG_H

#include "keybindwidget.h"
#include <QDialog>

namespace Ui {
class KeybindDialog;
}

class KeybindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeybindDialog(QWidget *parent, KeybindWidget *text);
    ~KeybindDialog();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::KeybindDialog *ui;
    KeybindWidget *text;
};

#endif // KEYBINDDIALOG_H
