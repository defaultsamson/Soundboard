#ifndef DIALOGKEYBIND_H
#define DIALOGKEYBIND_H

#include "keybindwidget.h"
#include <QDialog>

namespace Ui {
class DialogKeybind;
}

class DialogKeybind : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKeybind(QWidget *parent, KeybindWidget *text);
    ~DialogKeybind();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::DialogKeybind *ui;
    KeybindWidget *text;
};

#endif // DIALOGKEYBIND_H
