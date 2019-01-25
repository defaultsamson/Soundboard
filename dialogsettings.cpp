#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "mainwindow.h"

#include <QAudioDeviceInfo>
#include <QAudioDecoder>
#include <QBuffer>

DialogSettings::DialogSettings(MainWindow *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main),
    audio(nullptr)
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
    initializeAudio(ui->comboBoxOutputDevice->itemData(0).value<QAudioDeviceInfo>());
}

DialogSettings::~DialogSettings()
{
    delete ui;
    audio->stop();
    delete audio;
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
    this->deviceInfo = deviceInfo;
    QAudioFormat format = deviceInfo.preferredFormat();
    /*
    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    if (!deviceInfo.isFormatSupported(format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        format = deviceInfo.nearestFormat(format);
    }*/

    if (audio) delete audio;
    audio = new QAudioOutput(deviceInfo, format);
    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));

}

void DialogSettings::on_pushButtonOutput_clicked()
{
    // TEST SOUND
    qDebug() << "Testing audio";

    //audio->stop();
    //sourceFile.close();
    //sourceFile.setFileName("/home/samson/Desktop/queen/succ.wav");
    //sourceFile.open(QIODevice::ReadOnly);

    //audio->start(&sourceFile);
}

void DialogSettings::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            audio->stop();
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (audio->error() != QAudio::NoError) {
                // Error handling
            }
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}
