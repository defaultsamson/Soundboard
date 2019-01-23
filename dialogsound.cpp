#include "dialogsound.h"
#include "ui_dialogsound.h"

DialogSound::DialogSound(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSound)
{
    ui->setupUi(this);
}

DialogSound::~DialogSound()
{
    delete ui;
}
