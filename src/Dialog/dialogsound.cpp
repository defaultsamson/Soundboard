#include "dialogsound.h"
#include "ui_dialogsound.h"

#include "../mainapp.h"
#include "../settings.h"
#include "../Widget/listitemsound.h"
#include "dialogtestaudio.h"
#include "../Audio/audioobjectfile.h"

#include <QObject>
#include <QFileDialog>

DialogSound::DialogSound(Main* _main, ListItemSound* sound, bool creatingNew) :
    DialogTestAudio(_main),
    ui(new Ui::DialogSound),
    _main(_main),
    sound(sound),
    creatingNew(creatingNew),
    soundUpdated(false)
{
    ui->setupUi(this);
    ui->lineEditName->setText(creatingNew ? "" : sound->text());
    ui->keybindSound->updateKeyname(Settings::NON_NATIVE_KEYNAMING.value().toBool());
    if (sound->hasKey()) ui->keybindSound->setKey(sound->key());
    ui->lineEditFile->setText(sound->audio()->filename());
    ui->widgetVolume->setValue(sound->audio()->volumeInt());

    originalFileName = sound->audio()->filename();
    originalVolume = sound->audio()->volumeInt();

    updateTestButtons();

    sound->audio()->stop();

    // Restore the geometry, if it was saved
    if (Settings::REMEMBER_WINDOW_SIZES.value().toBool()) {
        if (Settings::WINDOW_SOUND_WIDTH.hasValue() && Settings::WINDOW_SOUND_HEIGHT.hasValue()) {
            resize(Settings::WINDOW_SOUND_WIDTH.value().toInt(), Settings::WINDOW_SOUND_HEIGHT.value().toInt());
        }
    }

    // Disable the keybinds temporarily while the dialog is up
    _main->disableKeybinds();

    // Sets up the audio visualizer
    connect(sound->audio(), &AudioObject::update, this, [this](qreal level) { ui->outputBar->setLevel(level); });
    sound->audio()->setUpdateVisualizer(true);

    // Connects the volume slider&box with the AudioObject
    connect(ui->widgetVolume, &WidgetVolume::valueChanged, sound->audio(), &AudioObject::setVolumeInt );

    _main->setAudioTestDialog(this);
    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));
}

DialogSound::~DialogSound()
{
    delete ui;
}

void DialogSound::on_buttonBox_accepted()
{
    QString originalName = sound->text();
    quint32 originalHasKey = sound->hasKey();
    quint32 originalKey = sound->key();
    // QString originalFilename = sound->filename();
    // int originalVolume = sound->volume();

    // Updates the values to the board
    sound->setText(ui->lineEditName->text().length() > 0 ? ui->lineEditName->text() : ListItemSound::NEW_SOUND);
    if (ui->keybindSound->hasKey()) sound->setKey(ui->keybindSound->key());
    else sound->unSetKey();
    sound->audio()->setFile(ui->lineEditFile->text());
    sound->audio()->setVolumeInt(ui->widgetVolume->value());

    // If anything's ACTUALLY changed, then tell the program
    if (creatingNew
            || sound->text() != originalName
            || sound->hasKey() != originalHasKey
            || sound->key() != originalKey
            || sound->audio()->filename() != originalFileName
            || sound->audio()->volumeInt() != originalVolume) {
        _main->setChanged();
    }

    soundUpdated = true;
    close();
}

void DialogSound::on_buttonBox_rejected()
{
    close();
}

void DialogSound::on_pushButtonFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load Audio File"), QString(), tr("(*.wav *.ogg *.flac *.aiff *.mp2)"));
    if (fn.isNull()) return;
    updateFileName(fn);
}

void DialogSound::on_lineEditFile_textEdited(const QString &fn) {
    updateFileName(fn);
}

void DialogSound::updateFileName(QString fn) {
    // test to see if the file exists and is readable
    sound->audio()->setFile(fn);
    ui->lineEditFile->setText(fn);
    updateTestButtons();
}

void DialogSound::onClose() {
    // Remove the board if it's being created new and wasn't saved (e.g. hit "OK: on)
    bool removed = false;
    if (creatingNew && !soundUpdated) {
       _main->removeSound(sound, true);
       removed = true;
    } else if (!creatingNew && !soundUpdated) {
        sound->audio()->setFile(originalFileName);
        sound->audio()->setVolumeInt(originalVolume);
    }

    if (!removed) {
        sound->audio()->stop();
        sound->audio()->setUpdateVisualizer(false);
    }

    // Re-enable the keybinds
    _main->enableKeybinds();

    _main->setAudioTestDialog(nullptr);

    // Save the geometry
    Settings::WINDOW_SOUND_WIDTH.setValue(width());
    Settings::WINDOW_SOUND_HEIGHT.setValue(height());
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
    bool enabled = _main->audio().isInitialized() && sound->audio()->hasFile();

    ui->pushButtonPlay->setEnabled(enabled);
    ui->pushButtonPause->setEnabled(enabled);
    ui->pushButtonStop->setEnabled(enabled);
}
