#ifndef DIALOGKEYBIND_H
#define DIALOGKEYBIND_H

#include "../Widget/widgetkeybind.h"
#include <QDialog>
#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class DialogKeybind;
}

class DialogKeybind : public QDialog
{
    Q_OBJECT

public:
    explicit DialogKeybind(QWidget* parent, WidgetKeybind* keybindWidget);
    ~DialogKeybind();

protected:
    void keyPressEvent(QKeyEvent* e);

private slots:
    void on_buttonUnbind_clicked();

private:
    Ui::DialogKeybind* ui;
    WidgetKeybind* widget;
};

#endif // DIALOGKEYBIND_H
