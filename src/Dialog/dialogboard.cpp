#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "../mainapp.h"
#include "../Widget/listitemboard.h"

#include <QObject>

DialogBoard::DialogBoard(Main *main, ListItemBoard *board, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogBoard),
    main(main),
    board(board),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    ui->lineEdiKeybind->setKey(board->key(), board->nativeKey());

    // Disable the keybinds temporarily while the dialog is up
    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogBoard::~DialogBoard()
{
    delete ui;
}

void DialogBoard::on_buttonBox_accepted()
{
    QString originalName = board->text();
    int originalKey = board->key();

    board->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    // TOD check if still needed board->setKey(ui->lineEdiKeybind->key());
    board->setNativeKey(ui->lineEdiKeybind->nativeKey());

    // If anything's ACTUALLY changed, then tell the program
    if (creatingNew
            || board->text() != originalName
            || board->key() != originalKey) {
        main->setChanged();
    }

    boardUpdated = true;
    close();
}

void DialogBoard::on_buttonBox_rejected()
{
    close();
}

void DialogBoard::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    if (creatingNew && !boardUpdated) {
       main->removeBoard(board, true);
    }
    // Re-enable the keybinds
    main->enableKeybinds();
}
