#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "../mainapp.h"
#include "../Widget/listitemboard.h"

#include <QObject>

DialogBoard::DialogBoard(Main* main, ListItemBoard* board, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogBoard),
    main(main),
    board(board),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    if (board->hasKey()) ui->keybindBoard->setKey(board->key());

    // Restore the geometry, if it was saved
    if (main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool())
        if (main->settings()->contains(Main::WINDOW_BOARD_GEOMETRY))
            restoreGeometry(main->settings()->value(Main::WINDOW_BOARD_GEOMETRY).toByteArray());

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
    quint32 originalHasKey = board->hasKey();
    quint32 originalKey = board->key();

    board->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    if (ui->keybindBoard->hasKey()) board->setKey(ui->keybindBoard->key());
    else board->unSetKey();

    // If anything's ACTUALLY changed, then tell the program
    if (creatingNew
            || board->text() != originalName
            || board->hasKey() != originalHasKey
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

    // Save the geometry
    main->settings()->setValue(Main::WINDOW_BOARD_GEOMETRY, saveGeometry());
}
