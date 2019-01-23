#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QLineEdit>
#include <QFocusEvent>

class WidgetKeybind : public QLineEdit
{
public:
    WidgetKeybind(QWidget *parent = nullptr);
    void setKey(int key);
    int key();

protected:
    void focusInEvent(QFocusEvent *) override;

private:
    int _key;
};

#endif // KEYBINDWIDGET_H
