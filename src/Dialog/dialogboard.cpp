#include "dialogboard.h"
#include "ui_dialogboard.h"

#include "../mainapp.h"
#include "../settings.h"

#include <QObject>

DialogBoard::DialogBoard(Main* _main, ListItemBoard* board, bool creatingNew) :
    QDialog(_main),
    ui(new Ui::DialogBoard),
    _main(_main),
    _board(board),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : board->text());
    if (board->hasKey()) ui->keybindBoard->setKey(board->key());

    // Restore the geometry, if it was saved
    if (Settings::REMEMBER_WINDOW_SIZES.value().toBool()) {
        if (Settings::WINDOW_BOARD_WIDTH.hasValue() && Settings::WINDOW_BOARD_HEIGHT.hasValue()) {
            resize(Settings::WINDOW_BOARD_WIDTH.value().toInt(), Settings::WINDOW_BOARD_HEIGHT.value().toInt());
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
    QString originalName = _board->text();
    quint32 originalHasKey = _board->hasKey();
    quint32 originalKey = _board->key();

    _board->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemBoard::NEW_BOARD);
    if (ui->keybindBoard->hasKey()) _board->setKey(ui->keybindBoard->key());
    else _board->unSetKey();

    // If anything's ACTUALLY changed, then tell the program
    if (creatingNew
            || _board->text() != originalName
            || _board->hasKey() != originalHasKey
            || _board->key() != originalKey) {
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
       _main->removeBoard(_board, true);
    }
    // Re-enable the keybinds
    _main->enableKeybinds();

    // Save the geometry
    Settings::WINDOW_BOARD_WIDTH.setValue(width());
    Settings::WINDOW_BOARD_HEIGHT.setValue(height());
}
