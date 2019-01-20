#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QLineEdit>

class KeybindWidget : public QLineEdit
{
public:
    KeybindWidget(QWidget *parent = nullptr);
    void updateKey(int key);
    int key;

protected:
    void focusInEvent(QFocusEvent *) override;
};

#endif // KEYBINDWIDGET_H
