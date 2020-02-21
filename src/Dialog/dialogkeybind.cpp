#include "dialogkeybind.h"
#include "ui_dialogkeybind.h"

#include "../Widget/widgetkeybind.h"
#include <QKeyEvent>

DialogKeybind::DialogKeybind(QWidget* parent, WidgetKeybind* keybindWidget) :
    QDialog(parent),
    ui(new Ui::DialogKeybind),
    widget(keybindWidget)
{
    ui->setupUi(this);
    ui->buttonUnbind->setEnabled(widget->hasKey());
}

DialogKeybind::~DialogKeybind()
{
    delete ui;
}

void DialogKeybind::keyPressEvent(QKeyEvent* e) {
    // When a key is presed, update the widget value and close the dialog
    widget->setKey(e->nativeScanCode());
    close();
}

void DialogKeybind::on_buttonUnbind_clicked()
{
    widget->unSetKey();
    close();
}
