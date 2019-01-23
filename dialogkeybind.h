#ifndef DIALOGKEYBIND_H
#define DIALOGKEYBIND_H

#include "widgetkeybind.h"
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
    explicit DialogKeybind(QWidget *parent, WidgetKeybind *text);
    ~DialogKeybind();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::DialogKeybind *ui;
    WidgetKeybind *widget;
};

#endif // DIALOGKEYBIND_H
