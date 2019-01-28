#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "mainwindow.h"

#include <QAudioDeviceInfo>
#include <QAudioDecoder>
#include <QBuffer>
#include <QObject>
#include <QProgressBar>
DialogSettings::DialogSettings(MainWindow *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);

    QComboBox *box = ui->comboBoxOutputDevice;
    const QAudioDeviceInfo &defaultDeviceInfo = QAudioDeviceInfo::defaultOutputDevice();
    box->addItem(defaultDeviceInfo.deviceName(), qVariantFromValue(defaultDeviceInfo));
    for (auto &deviceInfo: QAudioDeviceInfo::availableDevices(QAudio::AudioOutput)) {
        if (deviceInfo != defaultDeviceInfo)
            box->addItem(deviceInfo.deviceName(), qVariantFromValue(deviceInfo));
    }
    connect(box, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceChanged);

    // Initialize the default
    if (ui->comboBoxOutputDevice->count() > 0) {
        initializeAudio(ui->comboBoxOutputDevice->itemData(0).value<QAudioDeviceInfo>());
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
    initializeAudio(ui->comboBoxOutputDevice->itemData(index).value<QAudioDeviceInfo>());
}

void DialogSettings::initializeAudio(const QAudioDeviceInfo &deviceInfo)
{
    qDebug() << "Initializing audio...";
    audio.init(deviceInfo);
    audio.setFile("/home/samson/Desktop/succ.flac");
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
