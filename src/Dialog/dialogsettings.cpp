#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioobject.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>

// TODO allow the user to change these
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER 256

// Allows storing the const PaDeviceInfo* as QVariants
Q_DECLARE_METATYPE(DeviceInfoContainer)

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
        initializeAudio(ui->comboBoxOutputDevice->itemData(0).value<DeviceInfoContainer>());
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
    initializeAudio(ui->comboBoxOutputDevice->itemData(index).value<DeviceInfoContainer>());
}

void DialogSettings::initializeAudio(DeviceInfoContainer device)
{
    qDebug() << "Initializing audio...";
    PaStream *stream;
    PaError err;

    PaStreamParameters outputParameters;
    bzero( &outputParameters, sizeof( outputParameters ) ); //not necessary if you are filling in all the fields
    outputParameters.channelCount = device.info->maxOutputChannels;
    outputParameters.device = device.index;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = device.info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = NULL; //See you specific host's API docs for info on using this field

    err = Pa_OpenStream(
                    &stream,
                    nullptr,
                    &outputParameters,
                    device.info->defaultSampleRate,
                    FRAMES_PER_BUFFER,
                    paNoFlag, //flags that can be used to define dither, clip settings and more
                    nullptr, //your callback function
                    (void *) this); //data to be passed to callback. In C++, it is frequently (void *)this
    //don't forget to check errors!
    if (err != paNoError) qDebug() << "Error opening stream";

    /*
    audio.init(deviceInfo);
    audio.setFile("/home/samson/Desktop/succ.flac");
    // Sets up audio bar
    connect(&audio.stream(), &AudioFileStream::update, this, [&](qreal level) {
        ui->outputBar->setLevel(level);
    });*/
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
    for (DeviceInfoContainer deviceInfo: main->audio()->devices()) {
        // TODO if (deviceInfo != defaultDeviceInfo)
        box->addItem(deviceInfo.info->name, QVariant::fromValue(deviceInfo));
    }
}
