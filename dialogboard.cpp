#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "mainwindow.h"
#include "listitemboard.h"

#include <QKeyEvent>
#include <QObject>

DialogBoard::DialogBoard(MainWindow *main, ListItemBoard *board, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogBoard),
    main(main),
    board(board),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    ui->lineEdiKeybind->updateKey(board->key());

    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogBoard::~DialogBoard()
{
    delete ui;
}

// Save settings
void DialogBoard::on_pushButtonOK_clicked()
{
    this->board->setText(this->ui->lineEditName->text().length() > 0 ? this->ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    this->board->setKey(this->ui->lineEdiKeybind->key);
    didSomething = true;
    this->close();
}

void DialogBoard::on_pushButtonCancel_clicked()
{
    this->close();
}

void DialogBoard::onClose() {
    if (creatingNew && !didSomething) {
       main->removeBoard(board);
    }
    main->enableKeybinds();
}

