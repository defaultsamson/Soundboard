#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "board.h"

#include <QKeyEvent>

DialogBoard::DialogBoard(QWidget *parent, Board *board) :
    QDialog(parent),
    ui(new Ui::DialogBoard),
    board(board)
{
    ui->setupUi(this);
    ui->lineEditName->setText(!QString::compare(board->text(), "New Board", Qt::CaseInsensitive) ? "" : board->text());
    ui->lineEdiKeybind->updateKey(board->key());
}

DialogBoard::~DialogBoard()
{
    delete ui;
}

// Save settings
void DialogBoard::on_pushButtonOK_clicked()
{
    this->board->setText(this->ui->lineEditName->text().length() > 0 ? this->ui->lineEditName->text() : "New Board");
    this->board->setKey(this->ui->lineEdiKeybind->key);
    this->close();
}

void DialogBoard::on_pushButtonCancel_clicked()
{
    this->close();
}
