#include "dialogsound.h"
#include "ui_dialogsound.h"

#include "../mainapp.h"
#include "../Widget/listitemsound.h"

#include <QObject>
#include <QFileDialog>

DialogSound::DialogSound(Main *main, ListItemSound *sound, bool creatingNew) :
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
    ui->lineEditFile->setText(sound->filename());
    ui->sliderVolume->setValue(sound->volume());
    ui->spinBoxVolume->setValue(sound->volume());

    originalFileName = sound->filename();
    originalVolume = sound->volume();

    // Disable the keybinds temporarily while the dialog is up
    main->disableKeybinds();
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));

    connect(main->audio(), &AudioEngine::update, this, [&](qreal level) {
        ui->outputBar->setLevel(level);
    });
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
    sound->setFileName(ui->lineEditFile->text());
    sound->setVolume(ui->sliderVolume->value());
    soundUpdated = true;
    close();
}

void DialogSound::on_buttonBox_rejected()
{
    close();
}

void DialogSound::on_sliderVolume_valueChanged(int value)
{
    ui->spinBoxVolume->setValue(value);
    sound->setVolume(value);
}

void DialogSound::on_spinBoxVolume_valueChanged(int value)
{
    ui->sliderVolume->setValue(value);
    sound->setVolume(value);
}

void DialogSound::on_pushButtonFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load Audio File"), QString(), tr("(*.wav *.ogg *.flac)"));

    QFile file(fn);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open audio file.");
        return;
    }

    ui->lineEditFile->setText(fn);
    sound->setFileName(fn);
}

void DialogSound::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    if (creatingNew && !soundUpdated) {
       main->removeSound(sound);
    } else if (!creatingNew && !soundUpdated) {
        sound->setFileName(originalFileName);
        sound->setVolume(originalVolume);
    }
    sound->audio()->stop();
    // Re-enable the keybinds
    main->enableKeybinds();
}

void DialogSound::on_pushButton_2_clicked()
{
    sound->audio()->play();
}
