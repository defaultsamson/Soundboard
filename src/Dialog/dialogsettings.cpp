#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioobject.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>

// Allows storing the const PaDeviceInfo* as QVariants
Q_DECLARE_METATYPE(const PaDeviceInfo*)

DialogSettings::DialogSettings(Main *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);

    ui->checkBoxDarkTheme->setChecked(main->settings()->value(Main::DARK_THEME, false).toBool());

    refreshDeviceSelection();

    connect(ui->comboBoxOutputDevice, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceChanged);

    // Initialize the default
    if (ui->comboBoxOutputDevice->count() > 0) {
        initializeAudio(ui->comboBoxOutputDevice->itemData(0).value<const PaDeviceInfo *>());
    }
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_accepted()
{
    // TODO save
    close();
}

void DialogSettings::on_buttonBox_rejected()
{
    close();
}

void DialogSettings::deviceChanged(int index)
{
    initializeAudio(ui->comboBoxOutputDevice->itemData(index).value<const PaDeviceInfo *>());
}

void DialogSettings::initializeAudio(const PaDeviceInfo *deviceInfo)
{
    qDebug() << "Initializing audio...";
    audio.init(deviceInfo);
    audio.setFile("/home/samson/Desktop/succ.flac");
    // Sets up audio bar
    connect(&audio.stream(), &AudioFileStream::update, this, [&](qreal level) {
        ui->outputBar->setLevel(level);
    });
}

void DialogSettings::on_pushButtonOutput_clicked()
{
    qDebug() << "Testing audio";
    if (pause) {
        pause = false;
        audio.pause();
    } else {
        pause = true;
        audio.play();
    }
}

void DialogSettings::on_checkBoxDarkTheme_stateChanged(int /* arg1 */)
{
    main->setDarkTheme(ui->checkBoxDarkTheme->isChecked());
}

void DialogSettings::on_pushButtonRefresh_clicked() {
    qDebug() << "Refreshing devices...";
    main->audio()->refreshDevices();
    refreshDeviceSelection();
    qDebug() << "Finished updating device list";
}

void DialogSettings::refreshDeviceSelection() {
    // TODO get all the default info from the AudioEngine
    QComboBox *box = ui->comboBoxOutputDevice;
    // const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    // TODO box->addItem(defaultDeviceInfo.deviceName(), QVariant::fromValue(defaultDeviceInfo));
    for (const PaDeviceInfo *deviceInfo: main->audio()->devices()) {
        // TODO if (deviceInfo != defaultDeviceInfo)
        box->addItem(deviceInfo->name, QVariant::fromValue(deviceInfo));
    }
}
