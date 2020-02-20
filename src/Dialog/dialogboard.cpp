#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "../mainapp.h"
#include "../Widget/listitemboard.h"

#include <QObject>

DialogBoard::DialogBoard(Main* _main, ListItemBoard* board, bool creatingNew) :
    QDialog(_main),
    ui(new Ui::DialogBoard),
    _main(_main),
    board(board),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    if (board->hasKey()) ui->keybindBoard->setKey(board->key());

    // Restore the geometry, if it was saved
    if (_main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool()) {
        if (_main->settings()->contains(Main::WINDOW_BOARD_WIDTH)) {
            resize(_main->settings()->value(Main::WINDOW_BOARD_WIDTH, 500).toInt(), _main->settings()->value(Main::WINDOW_BOARD_HEIGHT, 500).toInt());
        }
    }

    // Disable the keybinds temporarily while the dialog is up
    _main->disableKeybinds();
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
        _main->setChanged();
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
       _main->removeBoard(board, true);
    }
    // Re-enable the keybinds
    _main->enableKeybinds();

    // Save the geometry
    _main->settings()->setValue(Main::WINDOW_BOARD_WIDTH, width());
    _main->settings()->setValue(Main::WINDOW_BOARD_HEIGHT, height());
}
