#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QLineEdit>
#include <QFocusEvent>

class WidgetKeybind : public QLineEdit
{
public:
    WidgetKeybind(QWidget *parent = nullptr);
    void updateKey(int key);
    int key;

protected:
    void focusInEvent(QFocusEvent *) override;
};

#endif // KEYBINDWIDGET_H
