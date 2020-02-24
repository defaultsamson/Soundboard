#ifndef DIALOGKEYBIND_H
#define DIALOGKEYBIND_H

#include "../Widget/widgetkeybind.h"
#include <QDialog>
#include <QWidget>
#include <QKeyEvent>
#include <QMutex>
#include <memory>

class NumlockThread;

namespace Ui {
class DialogKeybind;
}

class DialogKeybind : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKeybind(QWidget* parent, WidgetKeybind* keybindWidget);
    ~DialogKeybind();

    void setKey(quint32 nativeKeycode);

protected:
    void keyPressEvent(QKeyEvent* e);

private slots:
    void on_buttonUnbind_clicked();

private:
    Ui::DialogKeybind* ui;
    WidgetKeybind* widget;
    std::shared_ptr<NumlockThread> numlockThread;
    bool threadStarted = false;
    bool stopThreadFromSettingKey = false;
    std::shared_ptr<QMutex> _keyEventLock;
};

#endif // DIALOGKEYBIND_H
