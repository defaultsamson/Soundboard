#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioengine.h"
#include "../Audio/audioobject.h"
#include "dialogtestaudio.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>
#include <QStandardPaths>

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer*)
Q_DECLARE_METATYPE(DeviceInfoContainer*)

DialogSettings::DialogSettings(Main *main) :
    DialogTestAudio(main),
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

    connect(ui->comboBoxOutputDevice0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device0Changed);
    connect(ui->comboBoxDriver0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host0Changed);
    connect(ui->comboBoxOutputDevice1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device1Changed);
    connect(ui->comboBoxDriver1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host1Changed);

    audio.setFile(Main::TEST_AUDIO);
    main->audio()->registerAudio(&audio);

    connect(main->audio(), &AudioEngine::update, this, [&](qreal level) {
        ui->outputBar->setLevel(level);
    });

    main->setAudioTestDialog(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::handleClose() {
    main->audio()->unregisterAudio(&audio);
    main->setAudioTestDialog(nullptr);
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

void DialogSettings::host0Changed(int index)
{
    QVariant qvar = ui->comboBoxDriver0->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHost0 == host) return; // _displayHost0 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHost0 = host;
    refreshDeviceSelection();
}

void DialogSettings::device0Changed(int index)
{
    deviceChanged(ui->comboBoxOutputDevice0, index, 0, &_displayHost0);
}

void DialogSettings::host1Changed(int index)
{
    QVariant qvar = ui->comboBoxDriver1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHost1 == host) return; // _displayHost1 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHost1 = host;
    refreshDeviceSelection();
}

void DialogSettings::device1Changed(int index)
{
    deviceChanged(ui->comboBoxOutputDevice1, index, 1, &_displayHost1);
}

void DialogSettings::deviceChanged(QComboBox *selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    // Be cautious here, as dev may be null. only happens when "NONE" is selected
    DeviceInfoContainer* dev = qvar.value<DeviceInfoContainer*>();
    if (dev) {
        if (main->audio()->activeDevices().contains(dev)) return; // If it's already active, ignore
        dev->indexes.displayIndex = deviceDisplayIndex;
    }

    main->audio()->removeActiveDevice(deviceDisplayIndex);
    main->audio()->addActiveDevice(dev);

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    refreshDeviceSelection();

    switch (deviceDisplayIndex) {
    case 0:
        main->settings()->setValue(Main::DEVICE_INDEX0, dev ? dev->indexes.deviceIndex : -1);
        break;
    case 1:
        main->settings()->setValue(Main::DEVICE_INDEX1, dev ? dev->indexes.deviceIndex : -1);
        break;
    }

    main->settings()->setValue(Main::EXPLICIT_NO_DEVICES, main->audio()->activeDevices().count() == 0);
}

void DialogSettings::on_checkBoxDarkTheme_stateChanged(int /* arg1 */)
{
    main->setDarkTheme(ui->checkBoxDarkTheme->isChecked());
}

void DialogSettings::on_pushButtonRefresh_clicked() {
    main->audio()->refreshDevices();
    _displayHost1 = nullptr; // Forces a refresh of the list of hosts and devices
    refreshDeviceSelection();
}

void DialogSettings::audioEngineInit() {
    refreshDeviceSelection();
}

void DialogSettings::refreshDeviceSelection() {
    // TODO get all the default info from the AudioEngine
    QComboBox *driverBox = ui->comboBoxDriver0;
    QComboBox *deviceBox = ui->comboBoxOutputDevice0;
    driverBox->clear();
    deviceBox->clear();
    AudioEngine *a = main->audio();

    bool inited = a->isInitialized();
    ui->comboBoxDriver0->clear();
    ui->comboBoxDriver0->setEnabled(inited);
    ui->comboBoxOutputDevice0->clear();
    ui->comboBoxOutputDevice0->setEnabled(inited);
    ui->comboBoxDriver1->clear();
    ui->comboBoxDriver1->setEnabled(inited);
    ui->comboBoxOutputDevice1->clear();
    ui->comboBoxOutputDevice1->setEnabled(inited);
    ui->pushButtonPlay->setEnabled(inited && a->activeDevices().size() > 0);
    ui->pushButtonPause->setEnabled(inited && a->activeDevices().size() > 0);
    ui->pushButtonStop->setEnabled(inited && a->activeDevices().size() > 0);
    ui->pushButtonRefresh->setEnabled(inited);
    ui->groupBoxDevice0->setTitle(inited ? "Output Device 1" : "Output Device 1 (INITIALIZING...)");
    ui->groupBoxDevice1->setTitle(inited ? "Output Device 2" : "Output Device 2 (INITIALIZING...)");
    if (!inited) return;

    QList<AudioDisplayContainer> deviceDisplays;
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver0, ui->comboBoxOutputDevice0, _displayHost0, 0});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver1, ui->comboBoxOutputDevice1, _displayHost1, 1});
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
            // If the device hasn't already been added somewhere, and the display indexes match
            if (!displayedDevices.contains(dev) && display.deviceDisplayIndex == dev->indexes.displayIndex) {
                displayedDevices.append(dev);

                // If the active device is run by the displayed host (and if there is infact a display host)
                if (display.displayHost && display.displayHost != dev->host) {
                    // Act as though it's being displayed, even though the displayHost will be displayed.
                    // This is to prevent the next device display from showing this device when it's active
                    // in another device selector that's displaying a different host than the active one
                } else {
                    drivers->addItem(dev->host->name, QVariant::fromValue(dev->host));
                    devices->addItem(dev->info->name, QVariant::fromValue(dev));

                    displayHost = dev->host;
                }

                break; // Break so it only adds 1 as the active
            }
        }

        // If there were no active devices in the current display host,
        // and there is infact a current display host, then add it, it
        // wasn't added by the active device loop
        if (!displayHost && display.displayHost) {
            displayHost = display.displayHost;
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

            devices->addItem("NONE", QVariant::fromValue(nullptr));

            // Then add the default device
            if (a->defaultDevice() && !displayedDevices.contains(a->defaultDevice()) && displayHost == a->defaultDevice()->host) {
                // If the device is active
                if (a->activeDevices().contains(a->defaultDevice())) {
                    // Make sure it's not being displayed by another device display
                    if (display.deviceDisplayIndex == a->defaultDevice()->indexes.displayIndex) {
                        devices->addItem(a->defaultDevice()->info->name, QVariant::fromValue(a->defaultDevice()));
                    }
                } else {
                    devices->addItem(a->defaultDevice()->info->name, QVariant::fromValue(a->defaultDevice()));
                }
            }

            // Then add other devices
            for (DeviceInfoContainer* dev : a->devices()) {
                // If it's not already being displayed, it's not the default (because we just added it), and it's under this host
                if (!displayedDevices.contains(dev) && displayHost == dev->host && dev != a->defaultDevice()) {
                    // If the device is active
                    if (a->activeDevices().contains(dev)) {
                        // Make sure it's not being displayed by another device display
                        if (display.deviceDisplayIndex == dev->indexes.displayIndex) {
                            devices->addItem(dev->info->name, QVariant::fromValue(dev));
                        }
                    } else {
                        devices->addItem(dev->info->name, QVariant::fromValue(dev));
                    }
                }
            }
        }

        // A hack to forcefully update the visuals when updated from another thread
        // e.g. When the audio thread finished initializing the audio engine
        if (drivers->count() > 0) drivers->setCurrentIndex(0);
        if (devices->count() > 0) devices->setCurrentIndex(0);
    }
}

void DialogSettings::on_tabWidget_currentChanged(int index)
{
    main->settings()->setValue(Main::SETTINGS_TAB, index);
}

void DialogSettings::closeEvent(QCloseEvent *bar) {
    handleClose();
    bar->accept();
}

void DialogSettings::on_pushButtonPlay_clicked()
{
    audio.play();
}

void DialogSettings::on_pushButtonPause_clicked()
{
    audio.pause();
}

void DialogSettings::on_pushButtonStop_clicked()
{
    audio.stop();
}

void DialogSettings::setDeviceVolume(int value, int devDisplayIndex) {
    DeviceInfoContainer* dev = main->audio()->getActiveDevice(devDisplayIndex);
    if (dev) {
        dev->volumeInt = value;
        dev->volume = value / static_cast<float>(100);
    }
}

void DialogSettings::on_sliderDevice0_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice0->maximum() && ui->spinBoxDevice0->value() > value)) {
        ui->spinBoxDevice0->setValue(value);
        setDeviceVolume(value, 0);
    }
}

void DialogSettings::on_spinBoxDevice0_valueChanged(int value)
{
    ui->sliderDevice0->setValue(value);
    setDeviceVolume(value, 0);
}

void DialogSettings::on_sliderDevice1_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice1->maximum() && ui->spinBoxDevice1->value() > value)) {
        ui->spinBoxDevice1->setValue(value);
        setDeviceVolume(value, 1);
    }
}

void DialogSettings::on_spinBoxDevice1_valueChanged(int value)
{
    ui->sliderDevice1->setValue(value);
    setDeviceVolume(value, 1);
}

void DialogSettings::on_sliderInput_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderInput->maximum() && ui->spinBoxInput->value() > value)) {
        ui->spinBoxInput->setValue(value);
        setDeviceVolume(value, 2);
    }
}

void DialogSettings::on_spinBoxInput_valueChanged(int value)
{
    ui->sliderInput->setValue(value);
    setDeviceVolume(value, 3);
}

void DialogSettings::on_sliderTest_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderTest->maximum() && ui->spinBoxTest->value() > value)) {
        ui->spinBoxTest->setValue(value);
        audio.setVolume(value / static_cast<float>(100));
    }
}

void DialogSettings::on_spinBoxTest_valueChanged(int value)
{
    ui->sliderTest->setValue(value);
    audio.setVolume(value / static_cast<float>(100));
}
