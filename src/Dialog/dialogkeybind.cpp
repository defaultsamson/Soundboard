#include "dialogkeybind.h"
#include "ui_dialogkeybind.h"

#include "../Widget/widgetkeybind.h"
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
    if (e->key()==Qt::Key_Escape) {
        widget->unSetKey();
    } else {
        widget->setKey(e->nativeScanCode());
    }

    close();
}
