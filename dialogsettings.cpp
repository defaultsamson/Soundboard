#include "dialogsettings.h"
#include "ui_dialogsettings.h"

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_pushButton_2_clicked()
{
    this->close();
}

void DialogSettings::on_pushButton_clicked()
{
    // TODO save the settings
    this->close();
}
