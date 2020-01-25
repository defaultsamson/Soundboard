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

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer)
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
    connect(ui->comboBoxDriver, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::hostChanged);

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

void DialogSettings::hostChanged(int index)
{
    QVariant qvar = ui->comboBoxDriver->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    _displayHost = qvar.value<HostInfoContainer>();
    _hasDisplayHost = true;
    refreshDeviceSelection();
}

void DialogSettings::deviceChanged(int index)
{
    QVariant qvar = ui->comboBoxOutputDevice->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."
    main->audio()->setSelectedDevice(qvar.value<DeviceInfoContainer>());
    // If the host has been changed, update it too
    if (_hasDisplayHost) {
        main->audio()->setSelectedHost(_displayHost);
        _hasDisplayHost = false;
    }
    refreshDeviceSelection();
    initializeAudio(main->audio()->selectedDevice());
}

void DialogSettings::initializeAudio(DeviceInfoContainer device)
{
    qDebug() << "Initializing audio... " << main->audio()->activeHost().hostName;
    PaStream *stream;
    PaError err;

    PaStreamParameters outputParameters;
    bzero( &outputParameters, sizeof( outputParameters ) ); // not necessary if you are filling in all the fields
    outputParameters.channelCount = device.info->maxOutputChannels;
    outputParameters.device = device.index;
    outputParameters.sampleFormat = paFloat32;
    outputParameters.suggestedLatency = device.info->defaultLowOutputLatency ;
    outputParameters.hostApiSpecificStreamInfo = nullptr; // See your specific host's API docs for info on using this field

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
    main->audio()->refreshDevices();
    refreshDeviceSelection();
}

void DialogSettings::refreshDeviceSelection() {
    // TODO get all the default info from the AudioEngine
    QComboBox *driverBox = ui->comboBoxDriver;
    QComboBox *deviceBox = ui->comboBoxOutputDevice;
    driverBox->clear();
    deviceBox->clear();
    AudioEngine *a = main->audio();

    PaHostApiIndex showingHostIndex = -1;
    // Always prioritize the host being displayed over the active one
    if (_hasDisplayHost) {
        driverBox->addItem(_displayHost.hostName, QVariant::fromValue(_displayHost));
        showingHostIndex = _displayHost.hostIndex;
    } else if (a->hasActiveHost()) {
        driverBox->addItem(a->activeHost().hostName, QVariant::fromValue(a->activeHost()));
        showingHostIndex = a->activeHost().hostIndex;
        _displayHost = a->activeHost();
        _hasDisplayHost = true;
    } else {
        driverBox->addItem("Select backend...", QVariant(QVariant::Invalid));
    }
    for (HostInfoContainer hostInfo : a->hosts()) {
        if (!(_hasDisplayHost && hostInfo.hostIndex == showingHostIndex)) // If not the displaying host
            driverBox->addItem(hostInfo.hostName, QVariant::fromValue(hostInfo));
    }

    if (_hasDisplayHost) {
        // Only add default if it's showing the active device's tab
        if (a->hasActiveDevice() && _displayHost.hostIndex == a->activeDevice().info->hostApi) {
            deviceBox->addItem(a->activeDevice().info->name, QVariant::fromValue(a->activeDevice()));
        } else {
            deviceBox->addItem("Select device...", QVariant(QVariant::Invalid));
        }
        for (DeviceInfoContainer deviceInfo : *(_displayHost.devices)) {
            if (!(a->hasActiveDevice() && deviceInfo.index == a->activeDevice().index)) // If not the active host
                deviceBox->addItem(deviceInfo.info->name, QVariant::fromValue(deviceInfo));
        }
    }
}
