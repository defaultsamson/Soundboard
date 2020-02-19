#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QPushButton>
#include <QFocusEvent>

class WidgetKeybind : public QPushButton
{
public:
    WidgetKeybind(QWidget* parent = nullptr);
    void setKey(quint32 key);
    quint32 key();
    bool hasKey();
    void unSetKey();

private:
    quint32 _key;
    bool _hasKey;

private slots:
    void clickReciever();
};

#endif // KEYBINDWIDGET_H
