#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioobject.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>

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

#ifdef Q_OS_WIN
    audio.setFile("C:\\Users\\test\\Desktop\\test.ogg");
#else
    audio.setFile("/home/samson/Desktop/test.ogg");
#endif
    main->audio()->registerAudio(&audio);

    connect(main->audio(), &AudioEngine::update, this, [&](qreal level) {
        ui->outputBar->setLevel(level);
    });

    main->setSettingsDialog(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_buttonBox_accepted()
{
    // TODO save
    main->audio()->unregisterAudio(&audio);
    main->setSettingsDialog(nullptr);
    close();
}

void DialogSettings::on_buttonBox_rejected()
{
    main->audio()->unregisterAudio(&audio);
    main->setSettingsDialog(nullptr);
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
}

void DialogSettings::on_pushButtonOutput_clicked()
{
    if (audio.isPlaying()) {
        audio.pause();
    } else {
        audio.play();
    }
}

void DialogSettings::on_checkBoxDarkTheme_stateChanged(int /* arg1 */)
{
    main->setDarkTheme(ui->checkBoxDarkTheme->isChecked());
}

void DialogSettings::on_pushButtonRefresh_clicked() {
    main->audio()->refreshDevices();
    _hasDisplayHost = false; // Forces a refresh of the list of hosts and devices
    refreshDeviceSelection();
}

void DialogSettings::refreshDeviceSelection() {
    // TODO get all the default info from the AudioEngine
    QComboBox *driverBox = ui->comboBoxDriver;
    QComboBox *deviceBox = ui->comboBoxOutputDevice;
    driverBox->clear();
    deviceBox->clear();
    AudioEngine *a = main->audio();

    bool inited = a->isInitialized();
    driverBox->setEnabled(inited);
    deviceBox->setEnabled(inited);
    ui->pushButtonOutput->setEnabled(inited);
    ui->pushButtonRefresh->setEnabled(inited);
    ui->groupBox_2->setTitle(inited ? "Audio" : "Audio (INITIALIZING...)");
    if (!inited) return;

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
        deviceBox->setEnabled(false);
    }
    for (HostInfoContainer hostInfo : a->hosts()) {
        if (!(_hasDisplayHost && hostInfo.hostIndex == showingHostIndex)) // If not the displaying host
            driverBox->addItem(hostInfo.hostName, QVariant::fromValue(hostInfo));
    }

    if (_hasDisplayHost) {
        // Only add active device if it's showing the active device's tab
        bool addedActive = false;
        if (a->hasActiveDevice() && _displayHost.hostIndex == a->activeDevice().info->hostApi) {
            deviceBox->addItem(a->activeDevice().info->name, QVariant::fromValue(a->activeDevice()));
            addedActive = true;
        } else {
            deviceBox->addItem("Select device...", QVariant(QVariant::Invalid));
        }
        // Only add default device if it's showing the default device's tab and there is a default, and the active is not the default
        bool addedDefault = false;
        if (a->hasDefaultDevice() && _displayHost.hostIndex == a->defaultDevice().info->hostApi && !(a->hasActiveDevice() && a->defaultDevice().index == a->activeDevice().index)) {
            deviceBox->addItem(a->defaultDevice().info->name, QVariant::fromValue(a->defaultDevice()));
            addedDefault = true;
        }

        for (DeviceInfoContainer deviceInfo : *(_displayHost.devices)) {
            // If not the active host and not the default host, add the device to the list
            if (!(addedActive && deviceInfo.index == a->activeDevice().index) && !(addedDefault && deviceInfo.index == a->defaultDevice().index))
                deviceBox->addItem(deviceInfo.info->name, QVariant::fromValue(deviceInfo));
        }
    }

    ui->pushButtonOutput->setEnabled(a->hasActiveDevice());

    // A hack to forcefully update the visuals when updated from another thread
    // e.g. When the audio thread finished initializing the audio engine
    if (driverBox->count() > 0) driverBox->setCurrentIndex(0);
    if (deviceBox->count() > 0) deviceBox->setCurrentIndex(0);
}
