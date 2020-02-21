#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFocusEvent>

class WidgetKeybind : public QWidget
{
public:
    WidgetKeybind(QWidget* parent);
    void setKey(quint32 key);
    quint32 key();
    bool hasKey();
    void unSetKey();

private:
    quint32 _key;
    bool _hasKey;

    QPushButton *buttonKey;
    QPushButton *buttonDelete;

private slots:
    void onButtonKeyClicked();
    void onButtonDeleteClicked();
};

#endif // KEYBINDWIDGET_H
