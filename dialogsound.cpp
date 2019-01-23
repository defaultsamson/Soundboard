#include "dialogsound.h"
#include "ui_dialogsound.h"

#include "mainwindow.h"
#include "listitemsound.h"

#include <QObject>

DialogSound::DialogSound(MainWindow *main, ListItemSound *sound, bool creatingNew) :
    QDialog(main),
    ui(new Ui::DialogSound),
    sound(sound),
    main(main),
    creatingNew(creatingNew)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : sound->text());
    ui->lineEdiKeybind->updateKey(sound->key());

    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogSound::~DialogSound()
{
    delete ui;
}

// Save settings
void DialogSound::on_pushButtonOK_clicked()
{
    sound->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemSound::NEW_SOUND);
    sound->setKey(ui->lineEdiKeybind->key);
    didSomething = true;
    close();
}

void DialogSound::on_pushButtonCancel_clicked()
{
    close();
}

void DialogSound::onClose() {
    // Removes the board from the list if it's new and cancelled
    if (!didSomething && creatingNew) {
        main->removeSound(sound);
    }
    main->enableKeybinds();
}
