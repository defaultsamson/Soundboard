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
    creatingNew(creatingNew),
    boardUpdated(false)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    ui->lineEdiKeybind->updateKey(board->key());

    // Disable the keybinds temporarily while the dialog is up
    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogBoard::~DialogBoard()
{
    delete ui;
}

void DialogBoard::on_pushButtonOK_clicked()
{
    // Updates the values to the board
    board->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    board->setKey(ui->lineEdiKeybind->key);
    boardUpdated = true;
    close();
}

void DialogBoard::on_pushButtonCancel_clicked()
{
    close();
}

void DialogBoard::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    if (creatingNew && !boardUpdated) {
       main->removeBoard(board);
    }
    // Re-enable the keybinds
    main->enableKeybinds();
}

