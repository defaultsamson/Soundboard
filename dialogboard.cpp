#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "mainwindow.h"
#include "listitemboard.h"

#include <QKeyEvent>
#include <QObject>

DialogBoard::DialogBoard(MainWindow *main, ListItemBoard *board, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogBoard),
    board(board)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    ui->lineEdiKeybind->updateKey(board->key());

    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), main, SLOT(enableKeybinds()));
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

