#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "mainwindow.h"

DialogSettings::DialogSettings(MainWindow *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_pushButton_2_clicked()
{
    close();
}

void DialogSettings::on_pushButton_clicked()
{
    // TODO save the settings
    close();
}
