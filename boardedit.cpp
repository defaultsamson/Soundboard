#include "boardedit.h"
#include "ui_boardedit.h"
#include <QKeyEvent>

#include <iostream>

BoardEdit::BoardEdit(QWidget *parent, Board *board) :
    QDialog(parent),
    ui(new Ui::BoardEdit),
    board(board)
{
    ui->setupUi(this);
    ui->lineEditName->setText(board->text());
    ui->lineEdiKeybind->updateKey(board->key);
}

BoardEdit::~BoardEdit()
{
    delete ui;
}

// Save settings
void BoardEdit::on_pushButtonOK_clicked()
{
    this->board->setText(this->ui->lineEditName->text());
    this->board->key = this->ui->lineEdiKeybind->key;
    this->close();
}

void BoardEdit::on_pushButtonCancel_clicked()
{
    this->close();
}
