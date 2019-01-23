#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "mainwindow.h"
#include "listitemboard.h"

#include <QObject>

DialogBoard::DialogBoard(MainWindow *main, ListItemBoard *board, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogBoard),
    board(board),
    main(main),
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
    board->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    board->setKey(ui->lineEdiKeybind->key);
    didSomething = true;
    close();
}

void DialogBoard::on_pushButtonCancel_clicked()
{
    close();
}

void DialogBoard::onClose() {
    // Removes the board from the list if it's new and cancelled
    if (!didSomething && creatingNew) {
        main->removeBoard(board);
    }
    main->enableKeybinds();
}
