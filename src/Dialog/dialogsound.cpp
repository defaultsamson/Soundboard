#include "dialogsound.h"
#include "ui_dialogsound.h"

#include "../mainapp.h"
#include "../Widget/listitemsound.h"
#include "dialogtestaudio.h"

#include <QObject>
#include <QFileDialog>

DialogSound::DialogSound(Main* main, ListItemSound* sound, bool creatingNew) :
    DialogTestAudio(main),
    ui(new Ui::DialogSound),
    main(main),
    sound(sound),
    creatingNew(creatingNew),
    soundUpdated(false)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : sound->text());
    if (sound->hasKey()) ui->lineEdiKeybind->setKey(sound->key());
    ui->lineEditFile->setText(sound->filename());
    ui->spinBoxVolume->setValue(sound->volume());
    ui->sliderVolume->setValue(sound->volume());

    originalFileName = sound->filename();
    originalVolume = sound->volume();

    updateTestButtons();

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
    QString originalName = sound->text();
    quint32 originalKey = sound->key();
    // QString originalFilename = sound->filename();
    // int originalVolume = sound->volume();

    // Updates the values to the board
    sound->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemSound::NEW_SOUND);
    if (ui->lineEdiKeybind->hasKey()) sound->setKey(ui->lineEdiKeybind->key());
    else sound->unSetKey();
    sound->setFileName(ui->lineEditFile->text());
    sound->setVolume(ui->spinBoxVolume->value());

    // If anything's ACTUALLY changed, then tell the program
    if (creatingNew
            || sound->text() != originalName
            || sound->key() != originalKey
            || sound->filename() != originalFileName
            || sound->volume() != originalVolume) {
        main->setChanged();
    }

    soundUpdated = true;
    close();
}

void DialogSound::on_buttonBox_rejected()
{
    close();
}

void DialogSound::on_sliderVolume_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderVolume->maximum() && ui->spinBoxVolume->value() > value)) {
        ui->spinBoxVolume->setValue(value);
        sound->setVolume(value);
    }
}

void DialogSound::on_spinBoxVolume_valueChanged(int value)
{
    ui->sliderVolume->setValue(value);
    sound->setVolume(value);
}

void DialogSound::on_pushButtonFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load Audio File"), QString(), tr("(*.wav *.ogg *.flac)"));
    testFileName(fn);
}

void DialogSound::on_lineEditFile_textEdited(const QString &fn) {
    testFileName(fn);
}

void DialogSound::testFileName(QString fn) {
    // test to see if the file exists and is readable
    sound->setFileName(fn);
    ui->lineEditFile->setText(fn);
    updateTestButtons();
}

void DialogSound::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    if (creatingNew && !soundUpdated) {
       main->removeSound(sound, true);
    } else if (!creatingNew && !soundUpdated) {
        sound->setFileName(originalFileName);
        sound->setVolume(originalVolume);
    }
    sound->audio()->stop();
    // Re-enable the keybinds
    main->enableKeybinds();
}

void DialogSound::on_pushButtonPlay_clicked()
{
    sound->audio()->play();
}

void DialogSound::on_pushButtonPause_clicked()
{
    sound->audio()->pause();
}

void DialogSound::on_pushButtonStop_clicked()
{
    sound->audio()->stop();
}

void DialogSound::audioEngineInit() {
    updateTestButtons();
}

void DialogSound::updateTestButtons() {
    bool enabled = main->audio()->isInitialized() && sound->audio()->hasFile();

    ui->pushButtonPlay->setEnabled(enabled);
    ui->pushButtonPause->setEnabled(enabled);
    ui->pushButtonStop->setEnabled(enabled);
}
