#include "dialogsound.h"
#include "ui_dialogsound.h"

#include "mainwindow.h"
#include "listitemsound.h"

#include <QObject>

DialogSound::DialogSound(MainWindow *main, ListItemSound *sound, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogSound),
    main(main),
    sound(sound),
    creatingNew(creatingNew),
    soundUpdated(false)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : sound->text());
    ui->lineEdiKeybind->setKey(sound->key());

    // Disable the keybinds temporarily while the dialog is up
    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogSound::~DialogSound()
{
    delete ui;
}

void DialogSound::on_buttonBox_accepted()
{
    // Updates the values to the board
    sound->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemSound::NEW_SOUND);
    sound->setKey(ui->lineEdiKeybind->key());
    soundUpdated = true;
    close();
}

void DialogSound::on_buttonBox_rejected()
{
    close();
}

void DialogSound::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    if (creatingNew && !soundUpdated) {
       main->removeSound(sound);
    }
    // Re-enable the keybinds
    main->enableKeybinds();
}
