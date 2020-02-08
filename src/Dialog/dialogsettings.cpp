#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioengine.h"
#include "../Audio/audioobject.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>
#include <QStandardPaths>

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer*)
Q_DECLARE_METATYPE(DeviceInfoContainer*)

DialogSettings::DialogSettings(Main *main) :
    QDialog(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);

#ifdef Q_OS_WIN
    ui->checkBoxDarkTheme->hide();
#endif
    ui->checkBoxDarkTheme->setChecked(main->settings()->value(Main::DARK_THEME, false).toBool());
    ui->tabWidget->setCurrentIndex(main->settings()->value(Main::SETTINGS_TAB, 0).toInt());

    refreshDeviceSelection();

    connect(ui->comboBoxOutputDevice1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device1Changed);
    connect(ui->comboBoxDriver1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host1Changed);
    connect(ui->comboBoxOutputDevice2, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device2Changed);
    connect(ui->comboBoxDriver2, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host2Changed);

    audio.setFile(Main::TEST_AUDIO);
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

void DialogSettings::handleClose() {
    main->audio()->unregisterAudio(&audio);
    main->setSettingsDialog(nullptr);
}

void DialogSettings::on_buttonBox_accepted()
{
    // TODO save
    handleClose();
    close();
}

void DialogSettings::on_buttonBox_rejected()
{
    handleClose();
    close();
}

void DialogSettings::host1Changed(int index)
{
    QVariant qvar = ui->comboBoxDriver1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    _displayHost = qvar.value<HostInfoContainer*>();
    refreshDeviceSelection();
}

void DialogSettings::device1Changed(int index)
{
    
    
    QVariant qvar = ui->comboBoxOutputDevice1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."
    DeviceInfoContainer* dev = qvar.value<DeviceInfoContainer*>();
    if (main->audio()->activeDevices().contains(dev)) return; // If it's already active, ignore
    DeviceInfoContainer* prev = ui->comboBoxOutputDevice1->itemData(0).value<DeviceInfoContainer*>();
    main->audio()->removeActiveDevice(prev);
    main->audio()->addActiveDevice(dev);
    // If the host has been changed, update it too
    if (_displayHost) {
        main->audio()->addActiveHost(_displayHost);
        _displayHost = nullptr;
    }
    refreshDeviceSelection();
}

void DialogSettings::host2Changed(int index)
{
    /*
    QVariant qvar = ui->comboBoxDriver1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    _displayHost = qvar.value<HostInfoContainer*>();
    refreshDeviceSelection();
    */
}

void DialogSettings::device2Changed(int index)
{
    /*
    QVariant qvar = ui->comboBoxOutputDevice1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."
    main->audio()->addActiveDevice(qvar.value<DeviceInfoContainer*>());
    // If the host has been changed, update it too
    if (_displayHost) {
        main->audio()->addActiveHost(_displayHost);
        _displayHost = nullptr;
    }
    refreshDeviceSelection();
    */
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
    _displayHost = nullptr; // Forces a refresh of the list of hosts and devices
    refreshDeviceSelection();
}

void DialogSettings::refreshDeviceSelection() {
    // TODO get all the default info from the AudioEngine
    QComboBox *driverBox = ui->comboBoxDriver1;
    QComboBox *deviceBox = ui->comboBoxOutputDevice1;
    driverBox->clear();
    deviceBox->clear();
    AudioEngine *a = main->audio();

    bool inited = a->isInitialized();
    driverBox->setEnabled(inited);
    deviceBox->setEnabled(inited);
    ui->pushButtonOutput->setEnabled(inited);
    ui->pushButtonRefresh->setEnabled(inited);
    ui->groupBoxDevice1->setTitle(inited ? "Output Device 1" : "Output Device 1 (INITIALIZING...)");
    ui->groupBoxDevice2->setTitle(inited ? "Output Device 2" : "Output Device 2 (INITIALIZING...)");
    if (!inited) return;

    QList<AudioDisplayContainer> deviceDisplays;
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver1, ui->comboBoxOutputDevice1});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver2, ui->comboBoxOutputDevice2});
    QList<DeviceInfoContainer*> displayedDevices;

    for (AudioDisplayContainer display : deviceDisplays) {
        QComboBox *drivers = display.drivers;
        QComboBox *devices = display.devices;
        drivers->clear();
        devices->clear();
        drivers->setEnabled(true);
        devices->setEnabled(true);

        HostInfoContainer *displayHost = nullptr;
        bool notActiveDriver = false;

        // First add a single active device
        for (DeviceInfoContainer* dev : a->activeDevices()) {
            // If the active device is part of the displayed host (and if there is infact a display host)
            if (!displayedDevices.contains(dev) && !(_displayHost && _displayHost != dev->host)) {
                displayedDevices.append(dev);

                drivers->addItem(dev->host->name, QVariant::fromValue(dev->host));
                devices->addItem(dev->info->name, QVariant::fromValue(dev));

                displayHost = dev->host;

                break; // Break so it only adds 1 as the active
            }
        }

        // If there were no active devices in the current display host,
        // and there is infact a current display host, then add it, it
        // wasn't added by the active device loop
        if (!displayHost && _displayHost) {
            displayHost = _displayHost;
            notActiveDriver = true;
            drivers->addItem(displayHost->name, QVariant::fromValue(displayHost));
        }

        if (!displayHost) {
            drivers->addItem("Select driver...", QVariant(QVariant::Invalid));
            devices->setEnabled(false);
            // Display the available drivers
            for (HostInfoContainer* host : a->hosts()) {
                drivers->addItem(host->name, QVariant::fromValue(host));
            }
        } else {
            // Then add the other hosts
            for (HostInfoContainer* host : a->hosts()) {
                if (host != displayHost) drivers->addItem(host->name, QVariant::fromValue(host));
            }

            if (notActiveDriver)
                devices->addItem("Select device...", QVariant(QVariant::Invalid));

            // Then add the default device
            if (a->defaultDevice() && !displayedDevices.contains(a->defaultDevice()) && displayHost == a->defaultDevice()->host)
                devices->addItem(a->defaultDevice()->info->name, QVariant::fromValue(a->defaultDevice()));

            // Then add other devices
            for (DeviceInfoContainer* dev : a->devices()) {
                if (!displayedDevices.contains(dev) && displayHost == dev->host) {
                    devices->addItem(dev->info->name, QVariant::fromValue(dev));
                }
            }
        }
    }

    /*
    PaHostApiIndex showingHostIndex = -1;
    // Always prioritize the host being displayed over the active one
    if (_displayHost) {
        driverBox->addItem(_displayHost->hostName, QVariant::fromValue(_displayHost));
        showingHostIndex = _displayHost->hostIndex;
    } else if (a->activeHosts().size() > 0) {
        HostInfoContainer* host = a->activeHosts().at(0);
        driverBox->addItem(host->hostName, QVariant::fromValue(host));
        showingHostIndex = host->hostIndex;
        _displayHost = host;
    } else {
        driverBox->addItem("Select backend...", QVariant(QVariant::Invalid));
        deviceBox->setEnabled(false);
    }
    for (HostInfoContainer *hostInfo : a->hosts()) {
        if (!(_displayHost && hostInfo->hostIndex == showingHostIndex)) // If not the displaying host
            driverBox->addItem(hostInfo->hostName, QVariant::fromValue(hostInfo));
    }

    if (_displayHost) {
        // Only add active device if it's showing the active device's tab
        bool addedActive = false;
        if (a->activeDevices().size() > 0) {
            if (_displayHost->hostIndex == a->activeDevice()->info->hostApi) {
                deviceBox->addItem(a->activeDevice()->info->name, QVariant::fromValue(a->activeDevice()));
                addedActive = true;
            }
        } else {
            deviceBox->addItem("Select device...", QVariant(QVariant::Invalid));
        }
        // Only add default device if it's showing the default device's tab and there is a default, and the active is not the default
        bool addedDefault = false;
        if (a->defaultDevice() && _displayHost->hostIndex == a->defaultDevice()->info->hostApi && !(a->activeDevice() && a->defaultDevice()->index == a->activeDevice()->index)) {
            deviceBox->addItem(a->defaultDevice()->info->name, QVariant::fromValue(a->defaultDevice()));
            addedDefault = true;
        }

        for (DeviceInfoContainer *deviceInfo : *(_displayHost->devices)) {
            // If not the active host and not the default host, add the device to the list
            if (!(addedActive && deviceInfo->index == a->activeDevice()->index) && !(addedDefault && deviceInfo->index == a->defaultDevice()->index))
                deviceBox->addItem(deviceInfo->info->name, QVariant::fromValue(deviceInfo));
        }
    }

    ui->pushButtonOutput->setEnabled(a->activeDevice());
    */

    // A hack to forcefully update the visuals when updated from another thread
    // e.g. When the audio thread finished initializing the audio engine
    if (driverBox->count() > 0) driverBox->setCurrentIndex(0);
    if (deviceBox->count() > 0) deviceBox->setCurrentIndex(0);
}

void DialogSettings::on_tabWidget_currentChanged(int index)
{
    main->settings()->setValue(Main::SETTINGS_TAB, index);
}

void DialogSettings::closeEvent(QCloseEvent *bar) {
    handleClose();
    bar->accept();
}
