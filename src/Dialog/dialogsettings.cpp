#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioengine.h"
#include "../Audio/audioobjectfile.h"
#include "dialogtestaudio.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>

#include <portaudio.h>
#include <QStandardPaths>

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer*)
Q_DECLARE_METATYPE(Device*)

DialogSettings::DialogSettings(Main* main) :
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

    connect(ui->comboBoxDevice0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device0Changed);
    connect(ui->comboBoxDriver0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host0Changed);
    connect(ui->comboBoxDevice1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::device1Changed);
    connect(ui->comboBoxDriver1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::host1Changed);
    connect(ui->comboBoxDeviceInput, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceInputChanged);
    connect(ui->comboBoxDriverInput, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::hostInputChanged);

    ui->spinBoxDevice0->setValue(main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt());
    ui->sliderDevice0->setValue(main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt());
    ui->spinBoxDevice1->setValue(main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt());
    ui->sliderDevice1->setValue(main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt());
    ui->spinBoxInput->setValue(main->settings()->value(Main::INPUT_VOLUME0, 100).toInt());
    ui->sliderInput->setValue(main->settings()->value(Main::INPUT_VOLUME0, 100).toInt());

    ui->spinBoxTest->setValue(main->settings()->value(Main::TEST_VOLUME, 100).toInt());
    ui->sliderTest->setValue(main->settings()->value(Main::TEST_VOLUME, 100).toInt());

    ui->checkBoxInput0->setChecked(main->settings()->value(Main::INPUT_OUT0, false).toBool());
    ui->checkBoxInput1->setChecked(main->settings()->value(Main::INPUT_OUT1, false).toBool());

    audio.setFile(Main::TEST_AUDIO);
    audio.setVolume(main->settings()->value(Main::TEST_VOLUME, 100).toInt() / static_cast<float>(100));
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
    deviceChanged(ui->comboBoxDevice0, index, 0, false, &_displayHost0);
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
    deviceChanged(ui->comboBoxDevice1, index, 1, false, &_displayHost1);
}

void DialogSettings::hostInputChanged(int index)
{
    QVariant qvar = ui->comboBoxDriverInput->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHostInput == host) return; // _displayHost1 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHostInput = host;
    refreshDeviceSelection();
}

void DialogSettings::deviceInputChanged(int index)
{
    deviceChanged(ui->comboBoxDeviceInput, index, 0, true, &_displayHostInput);
}

void DialogSettings::deviceChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, bool isInput, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    Device* dev = qvar.value<Device*>();
    if (isInput ? main->audio()->activeInputs().contains(dev) : main->audio()->activeOutputs().contains(dev)) return; // If it's already active, ignore
    dev->indexes()->displayIndex = deviceDisplayIndex;

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    if (isInput) main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    else main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);
    main->audio()->addActiveDevice(dev);

    if (isInput) {
        switch (deviceDisplayIndex) {
        case 0:
            main->settings()->setValue(Main::INPUT_INDEX0, dev->indexes()->deviceIndex);
            break;
        }
        main->settings()->setValue(Main::EXPLICIT_NO_INPUT_DEVICES, main->audio()->activeInputs().count() == 0);
    } else {
        switch (deviceDisplayIndex) {
        case 0:
            main->settings()->setValue(Main::OUTPUT_INDEX0, dev->indexes()->deviceIndex);
            break;
        case 1:
            main->settings()->setValue(Main::OUTPUT_INDEX1, dev->indexes()->deviceIndex);
            break;
        }
        main->settings()->setValue(Main::EXPLICIT_NO_OUTPUT_DEVICES, main->audio()->activeOutputs().count() == 0);
    }

    refreshDeviceSelection();
}

void DialogSettings::deviceRemoved(int deviceDisplayIndex, bool isInput, HostInfoContainer** displayHost)
{
    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    if (isInput) main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    else main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);

    if (isInput) {
        switch (deviceDisplayIndex) {
        case 0:
            main->settings()->setValue(Main::INPUT_INDEX0, -1);
            break;
        }
        main->settings()->setValue(Main::EXPLICIT_NO_INPUT_DEVICES, main->audio()->activeInputs().count() == 0);
    } else {
        switch (deviceDisplayIndex) {
        case 0:
            main->settings()->setValue(Main::OUTPUT_INDEX0, -1);
            break;
        case 1:
            main->settings()->setValue(Main::OUTPUT_INDEX1, -1);
            break;
        }
        main->settings()->setValue(Main::EXPLICIT_NO_OUTPUT_DEVICES, main->audio()->activeOutputs().count() == 0);
    }

    refreshDeviceSelection();
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
    AudioEngine* a = main->audio();

    bool inited = a->isInitialized();
    ui->groupBoxDevice0->setTitle(inited ? "Output Device 1" : "Output Device 1 (INITIALIZING...)");
    ui->groupBoxDevice1->setTitle(inited ? "Output Device 2" : "Output Device 2 (INITIALIZING...)");
    ui->groupBoxDeviceInput->setTitle(inited ? "Input Device" : "Input Device (INITIALIZING...)");
    ui->pushButtonPlay->setEnabled(inited && a->activeOutputs().size() > 0);
    ui->pushButtonPause->setEnabled(inited && a->activeOutputs().size() > 0);
    ui->pushButtonStop->setEnabled(inited && a->activeOutputs().size() > 0);
    ui->pushButtonRefresh->setEnabled(inited);
    ui->checkBoxInput0->setEnabled(inited);
    ui->checkBoxInput1->setEnabled(inited);
    if (!inited) {
        ui->comboBoxDriver0->clear();
        ui->comboBoxDriver0->setEnabled(false);
        ui->comboBoxDevice0->clear();
        ui->comboBoxDevice0->setEnabled(false);
        ui->comboBoxDriver1->clear();
        ui->comboBoxDriver1->setEnabled(false);
        ui->comboBoxDevice1->clear();
        ui->comboBoxDevice1->setEnabled(false);
        ui->comboBoxDriverInput->clear();
        ui->comboBoxDriverInput->setEnabled(false);
        ui->comboBoxDeviceInput->clear();
        ui->comboBoxDeviceInput->setEnabled(false);
        ui->deleteButtonDevice0->setEnabled(false);
        ui->deleteButtonDevice1->setEnabled(false);
        ui->deleteButtonDeviceInput->setEnabled(false);
        return;
    }

    /*
    if (a->getActiveDisplayOutput(0)) ui->spinBoxDevice0->setValue(a->getActiveDisplayOutput(0)->volumeInt());
    if (a->getActiveDisplayOutput(0)) ui->sliderDevice0->setValue(a->getActiveDisplayOutput(0)->volumeInt());
    if (a->getActiveDisplayOutput(1)) ui->spinBoxDevice1->setValue(a->getActiveDisplayOutput(1)->volumeInt());
    if (a->getActiveDisplayOutput(1)) ui->sliderDevice1->setValue(a->getActiveDisplayOutput(1)->volumeInt());
    if (a->getActiveDisplayInput(0)) ui->spinBoxInput->setValue(a->getActiveDisplayInput(0)->volumeInt());
    if (a->getActiveDisplayInput(0)) ui->sliderInput->setValue(a->getActiveDisplayInput(0)->volumeInt());
    */

    ui->checkBoxInput0->setChecked(a->inputObject()->isActiveOutput0());
    ui->checkBoxInput1->setChecked(a->inputObject()->isActiveOutput1());

    QList<AudioDisplayContainer> deviceDisplays;
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver0, ui->comboBoxDevice0, ui->deleteButtonDevice0, _displayHost0, 0, false});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriver1, ui->comboBoxDevice1, ui->deleteButtonDevice1, _displayHost1, 1, false});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriverInput, ui->comboBoxDeviceInput, ui->deleteButtonDeviceInput, _displayHostInput, 0, true});
    QList<Device*> displayedDevices;

    for (AudioDisplayContainer display : deviceDisplays) {
        QComboBox* drivers = display.drivers;
        QComboBox* devices = display.devices;
        drivers->clear();
        devices->clear();
        drivers->setEnabled(true);
        devices->setEnabled(true);
        display.deleteButton->setEnabled(true);

        HostInfoContainer* displayHost = nullptr;
        bool notActiveDriver = false;

        // First add a single active device
        for (Device* dev : display.isInput ? a->activeInputs() : a->activeOutputs()) {
            // If the device hasn't already been added somewhere, and the display indexes match
            if (!displayedDevices.contains(dev) && display.deviceDisplayIndex == dev->indexes()->displayIndex) {
                displayedDevices.append(dev);

                // If the active device is run by the displayed host (and if there is infact a display host)
                if (display.displayHost && display.displayHost != dev->host()) {
                    // Act as though it's being displayed, even though the displayHost will be displayed.
                    // This is to prevent the next device display from showing this device when it's active
                    // in another device selector that's displaying a different host than the active one
                } else {
                    drivers->addItem(dev->host()->name, QVariant::fromValue(dev->host()));
                    devices->addItem(dev->info()->name, QVariant::fromValue(dev));

                    displayHost = dev->host();
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
            display.deleteButton->setEnabled(false);
            // Display the available drivers
            for (HostInfoContainer* host : a->hosts()) {
                drivers->addItem(host->name, QVariant::fromValue(host));
            }
        } else {
            // Then add the other hosts
            for (HostInfoContainer* host : a->hosts()) {
                if (host != displayHost) drivers->addItem(host->name, QVariant::fromValue(host));
            }

            if (notActiveDriver) {
                devices->addItem("Select device...", QVariant(QVariant::Invalid));
                display.deleteButton->setEnabled(false);
            }

            // Then add the default device
            if (display.isInput
                    ? a->defaultInput() && !displayedDevices.contains(a->defaultInput()) && displayHost == a->defaultInput()->host()
                    : a->defaultOutput() && !displayedDevices.contains(a->defaultOutput()) && displayHost == a->defaultOutput()->host()) {
                // If the device is active
                if (display.isInput
                        ? a->activeInputs().contains(a->defaultInput())
                        : a->activeOutputs().contains(a->defaultOutput())) {
                    // Make sure it's not being displayed by another device display
                    if (display.isInput
                            ? display.deviceDisplayIndex == a->defaultInput()->indexes()->displayIndex
                            : display.deviceDisplayIndex == a->defaultOutput()->indexes()->displayIndex) {
                        devices->addItem(a->defaultOutput()->info()->name, QVariant::fromValue(a->defaultOutput()));
                    }
                } else {
                    devices->addItem(display.isInput ? a->defaultInput()->info()->name : a->defaultOutput()->info()->name,
                                     QVariant::fromValue(display.isInput ? a->defaultInput() : a->defaultOutput()));
                }
            }

            // Then add other devices
            for (Device* dev : display.isInput ? a->inputs() : a->outputs()) {
                // If it's not already being displayed, it's not the default (because we just added it), and it's under this host
                if (!displayedDevices.contains(dev) && displayHost == dev->host() && dev != (display.isInput ? a->defaultInput() : a->defaultOutput())) {
                    // If the device is active
                    if (display.isInput ? a->activeInputs().contains(dev) : a->activeOutputs().contains(dev)) {
                        // Make sure it's not being displayed by another device display
                        if (display.deviceDisplayIndex == dev->indexes()->displayIndex) {
                            devices->addItem(dev->info()->name, QVariant::fromValue(dev));
                        }
                    } else {
                        devices->addItem(dev->info()->name, QVariant::fromValue(dev));
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

void DialogSettings::closeEvent(QCloseEvent* bar) {
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
    Device* dev = main->audio()->getActiveDisplayOutput(devDisplayIndex);
    if (dev) dev->setVolume(value);
}

void DialogSettings::on_sliderDevice0_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice0->maximum() && ui->spinBoxDevice0->value() > value)) {
        ui->spinBoxDevice0->setValue(value);
        setDeviceVolume(value, 0);
        main->settings()->setValue(Main::OUTPUT_VOLUME0, value);
    }
}

void DialogSettings::on_spinBoxDevice0_valueChanged(int value)
{
    ui->sliderDevice0->setValue(value);
    setDeviceVolume(value, 0);
    main->settings()->setValue(Main::OUTPUT_VOLUME0, value);
}

void DialogSettings::on_sliderDevice1_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice1->maximum() && ui->spinBoxDevice1->value() > value)) {
        ui->spinBoxDevice1->setValue(value);
        setDeviceVolume(value, 1);
        main->settings()->setValue(Main::OUTPUT_VOLUME1, value);
    }
}

void DialogSettings::on_spinBoxDevice1_valueChanged(int value)
{
    ui->sliderDevice1->setValue(value);
    setDeviceVolume(value, 1);
    main->settings()->setValue(Main::OUTPUT_VOLUME1, value);
}

void DialogSettings::on_sliderInput_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderInput->maximum() && ui->spinBoxInput->value() > value)) {
        ui->spinBoxInput->setValue(value);
        setDeviceVolume(value, 2);
        main->settings()->setValue(Main::INPUT_VOLUME0, value);
    }
}

void DialogSettings::on_spinBoxInput_valueChanged(int value)
{
    ui->sliderInput->setValue(value);
    setDeviceVolume(value, 2);
    main->settings()->setValue(Main::INPUT_VOLUME0, value);
}

void DialogSettings::on_sliderTest_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderTest->maximum() && ui->spinBoxTest->value() > value)) {
        ui->spinBoxTest->setValue(value);
        audio.setVolume(value / static_cast<float>(100));
        main->settings()->setValue(Main::TEST_VOLUME, value);
    }
}

void DialogSettings::on_spinBoxTest_valueChanged(int value)
{
    ui->sliderTest->setValue(value);
    audio.setVolume(value / static_cast<float>(100));
    main->settings()->setValue(Main::TEST_VOLUME, value);
}

void DialogSettings::on_deleteButtonDevice0_clicked()
{
    deviceRemoved(0, false, &_displayHost0);
}

void DialogSettings::on_deleteButtonDevice1_clicked()
{
    deviceRemoved(1, false, &_displayHost1);
}

void DialogSettings::on_deleteButtonDeviceInput_clicked()
{
    deviceRemoved(0, true, &_displayHostInput);
}

void DialogSettings::on_checkBoxInput0_clicked()
{
    bool enabled = ui->checkBoxInput0->isChecked();
    main->settings()->setValue(Main::INPUT_OUT0, enabled);
    main->audio()->inputObject()->setOutput0(enabled);
}

void DialogSettings::on_checkBoxInput1_clicked()
{
    bool enabled = ui->checkBoxInput1->isChecked();
    main->settings()->setValue(Main::INPUT_OUT1, enabled);
    main->audio()->inputObject()->setOutput1(enabled);
}
