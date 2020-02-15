#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QLineEdit>
#include <QFocusEvent>

class WidgetKeybind : public QLineEdit
{
public:
    WidgetKeybind(QWidget *parent = nullptr);
    void setKey(quint32 key);
    quint32 key();
    bool hasKey();
    void unSetKey();

protected:
    void focusInEvent(QFocusEvent *) override;

private:
    quint32 _key;
    bool _hasKey;
};

#endif // KEYBINDWIDGET_H
