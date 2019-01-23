#include "dialogkeybind.h"
#include "ui_dialogkeybind.h"

#include "widgetkeybind.h"
#include <QKeyEvent>

DialogKeybind::DialogKeybind(QWidget *parent, WidgetKeybind *widget) :
    QDialog(parent),
    ui(new Ui::DialogKeybind),
    widget(widget)
{
    ui->setupUi(this);
}

DialogKeybind::~DialogKeybind()
{
    delete ui;
}

void DialogKeybind::keyPressEvent(QKeyEvent *e) {
    // When a key is presed, update the widget value and close the dialog
    widget->setKey(e->key()==Qt::Key_Escape ? -1 : e->key());
    close();
}
