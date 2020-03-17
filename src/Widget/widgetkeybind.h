#ifndef KEYBINDWIDGET_H
#define KEYBINDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFocusEvent>

class WidgetKeybind : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetKeybind(QWidget* parent = nullptr);
    void setKey(quint32 key);
    quint32 key();
    bool hasKey();
    void unSetKey();
    void updateKeyname(bool nonNative);

private:
    quint32 _key;
    bool _hasKey;
    bool _nonNative;

    QPushButton *buttonKey;
    QPushButton *buttonDelete;

private slots:
    void onButtonKeyClicked();
    void onButtonDeleteClicked();
};

#endif // KEYBINDWIDGET_H
