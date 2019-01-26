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
    main(main),
    audio(nullptr),
    stream(nullptr)
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

    if (stream) delete stream;
    stream = new AudioFileStream();
    if (!stream->init(format))
    {
        qWarning() << "Failed to init audio stream";
        delete stream;
        return;
    }

    connect(stream, &AudioFileStream::update, this, [&]() {
        ui->outputBar->setLevel(stream->level());
    });
    if (audio) delete audio;
    audio = new QAudioOutput(deviceInfo, format);
    audio->start(stream);
}

void DialogSettings::on_pushButtonOutput_clicked()
{
    // TEST SOUND
    qDebug() << "Testing audio";
    stream->stop();
    stream->play("/home/samson/Desktop/succ.flac");
}
