#include "dialogsettings.h"
#include "ui_dialogsettings.h"
#include "../mainapp.h"
#include "../Audio/audioengine.h"
#include "../Audio/audioobjectfile.h"
#include "dialogtestaudio.h"

#include <QBuffer>
#include <QObject>
#include <QProgressBar>
#include <QStandardPaths>
#include <QFileDialog>

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer*)
Q_DECLARE_METATYPE(Device*)

DialogSettings::DialogSettings(Main* main) :
    DialogTestAudio(main),
    ui(new Ui::DialogSettings),
    main(main)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(main->settings()->value(Main::SETTINGS_TAB, 0).toInt());

#ifdef Q_OS_WIN
    ui->checkBoxDarkTheme->hide();
#endif
    ui->checkBoxDarkTheme->setChecked(main->settings()->value(Main::DARK_THEME, false).toBool());
    ui->checkBoxShowMuteButton->setChecked(main->settings()->value(Main::SHOW_MUTE_BUTTON, true).toBool());
    ui->checkBoxWindowSize->setChecked(main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool());
    // Restore the geometry, if it was saved
    if (main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool())
        if (main->settings()->contains(Main::WINDOW_SETTINGS_GEOMETRY))
            restoreGeometry(main->settings()->value(Main::WINDOW_SETTINGS_GEOMETRY).toByteArray());

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

    QString testFile = main->settings()->value(Main::TEST_FILE, Main::DEFAULT_TEST_FILE).toString();
    audio.setFile(testFile);
    audio.setVolume(main->settings()->value(Main::TEST_VOLUME, 100).toInt() / static_cast<float>(100));
    audio.setUpdateVisualizer(true);
    connect(&audio, &AudioObject::update, this, [&](float level) {
        ui->outputBar->setLevel(static_cast<qreal>(level));
    });
    main->audio()->registerAudio(&audio);
    if (audio.hasFile()) ui->lineEditTestFile->setText(testFile);

    refreshDeviceSelection();

    main->setAudioTestDialog(this);
    main->disableKeybinds();

    // Load keybinds
    main->hkEnableKeybinds->unreg();
    if (main->hkEnableKeybinds->hasKey()) ui->keybindEnableKeybinds->setKey(main->hkEnableKeybinds->key());
    main->hkDisableKeybinds->unreg();
    if (main->hkDisableKeybinds->hasKey()) ui->keybindDisableKeybinds->setKey(main->hkDisableKeybinds->key());
    main->hkStopSounds->unreg();
    if (main->hkStopSounds->hasKey()) ui->keybindStopSounds->setKey(main->hkStopSounds->key());
    main->hkPauseSounds->unreg();
    if (main->hkPauseSounds->hasKey()) ui->keybindPauseSounds->setKey(main->hkPauseSounds->key());
    main->hkResumeSounds->unreg();
    if (main->hkResumeSounds->hasKey()) ui->keybindResumeSounds->setKey(main->hkResumeSounds->key());
    main->hkMuteInput->unreg();
    if (main->hkMuteInput->hasKey()) ui->keybindMuteInput->setKey(main->hkMuteInput->key());
    main->hkUnmuteInput->unreg();
    if (main->hkUnmuteInput->hasKey()) ui->keybindUnmuteInput->setKey(main->hkUnmuteInput->key());
    main->hkToggleMuteInput->unreg();
    if (main->hkToggleMuteInput->hasKey()) ui->keybindToggleMuteInput->setKey(main->hkToggleMuteInput->key());
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::handleClose() {
    main->audio()->unregisterAudio(&audio);
    main->setAudioTestDialog(nullptr);
    main->enableKeybinds();
    if (_inputObjectInited) main->audio()->inputObject()->setUpdateVisualizer(false);

    // Save the geometry
    main->settings()->setValue(Main::REMEMBER_WINDOW_SIZES, ui->checkBoxWindowSize->isChecked());
    main->settings()->setValue(Main::WINDOW_SETTINGS_GEOMETRY, saveGeometry());

    main->updateMuteButton();

    // Save all global keybinds
    bool hasKey = ui->keybindEnableKeybinds->hasKey();
    quint32 key = ui->keybindEnableKeybinds->key();
    main->settings()->setValue(Main::HK_ENABLE_KEYBINDS_HAS, hasKey);
    if (hasKey) {
        main->hkEnableKeybinds->setKey(ui->keybindEnableKeybinds->key());
        main->hkEnableKeybinds->reg();
        main->settings()->setValue(Main::HK_ENABLE_KEYBINDS_KEY, key);
    } else {
        main->hkEnableKeybinds->unSetKey();
    }


    hasKey = ui->keybindDisableKeybinds->hasKey();
    key = ui->keybindDisableKeybinds->key();
    main->settings()->setValue(Main::HK_DISABLE_KEYBINDS_HAS, hasKey);
    if (hasKey) {
        main->hkDisableKeybinds->setKey(ui->keybindDisableKeybinds->key());
        main->hkDisableKeybinds->reg();
        main->settings()->setValue(Main::HK_DISABLE_KEYBINDS_KEY, key);
    } else {
        main->hkDisableKeybinds->unSetKey();
    }


    hasKey = ui->keybindStopSounds->hasKey();
    key = ui->keybindStopSounds->key();
    main->settings()->setValue(Main::HK_STOP_SOUNDS_HAS, hasKey);
    if (hasKey) {
        main->hkStopSounds->setKey(ui->keybindStopSounds->key());
        main->hkStopSounds->reg();
        main->settings()->setValue(Main::HK_STOP_SOUNDS_KEY, key);
    } else {
        main->hkStopSounds->unSetKey();
    }


    hasKey = ui->keybindPauseSounds->hasKey();
    key = ui->keybindPauseSounds->key();
    main->settings()->setValue(Main::HK_PAUSE_SOUNDS_HAS, hasKey);
    if (hasKey) {
        main->hkPauseSounds->setKey(ui->keybindPauseSounds->key());
        main->hkPauseSounds->reg();
        main->settings()->setValue(Main::HK_PAUSE_SOUNDS_KEY, key);
    } else {
        main->hkPauseSounds->unSetKey();
    }


    hasKey = ui->keybindResumeSounds->hasKey();
    key = ui->keybindResumeSounds->key();
    main->settings()->setValue(Main::HK_RESUME_SOUNDS_HAS, hasKey);
    if (hasKey) {
        main->hkResumeSounds->setKey(ui->keybindResumeSounds->key());
        main->hkResumeSounds->reg();
        main->settings()->setValue(Main::HK_RESUME_SOUNDS_KEY, key);
    } else {
        main->hkResumeSounds->unSetKey();
    }


    hasKey = ui->keybindMuteInput->hasKey();
    key = ui->keybindMuteInput->key();
    main->settings()->setValue(Main::HK_MUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        main->hkMuteInput->setKey(ui->keybindMuteInput->key());
        main->hkMuteInput->reg();
        main->settings()->setValue(Main::HK_MUTE_INPUT_KEY, key);
    } else {
        main->hkMuteInput->unSetKey();
    }


    hasKey = ui->keybindUnmuteInput->hasKey();
    key = ui->keybindUnmuteInput->key();
    main->settings()->setValue(Main::HK_UNMUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        main->hkUnmuteInput->setKey(ui->keybindUnmuteInput->key());
        main->hkUnmuteInput->reg();
        main->settings()->setValue(Main::HK_UNMUTE_INPUT_KEY, key);
    } else {
        main->hkUnmuteInput->unSetKey();
    }


    hasKey = ui->keybindToggleMuteInput->hasKey();
    key = ui->keybindToggleMuteInput->key();
    main->settings()->setValue(Main::HK_TOGGLE_MUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        main->hkToggleMuteInput->setKey(ui->keybindToggleMuteInput->key());
        main->hkToggleMuteInput->reg();
        main->settings()->setValue(Main::HK_TOGGLE_MUTE_INPUT_KEY, key);
    } else {
        main->hkToggleMuteInput->unSetKey();
    }
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
    outputChanged(ui->comboBoxDevice0, index, 0, &_displayHost0);
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
    outputChanged(ui->comboBoxDevice1, index, 1, &_displayHost1);
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
    inputChanged(ui->comboBoxDeviceInput, index, 0, &_displayHostInput);
}

void DialogSettings::outputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    Device* dev = qvar.value<Device*>();
    if (main->audio()->activeOutputs().contains(dev)) return; // If it's already active, ignore
    dev->indexes()->outputDisplayIndex = deviceDisplayIndex;

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);
    main->audio()->addActiveOutput(dev);
    main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}
void DialogSettings::inputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    Device* dev = qvar.value<Device*>();
    if (main->audio()->activeInputs().contains(dev)) return; // If it's already active, ignore
    dev->indexes()->inputDisplayIndex = deviceDisplayIndex;

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    main->audio()->addActiveInput(dev);
    main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}

void DialogSettings::outputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);
    main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}
void DialogSettings::inputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    main->audio()->updateSavedDevices();

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

    if (main->settings()->value(Main::SHOW_DRIVERS, false).toBool()) {
        ui->labelDriverDevice0->show();
        ui->comboBoxDriver0->show();
        ui->labelDriverDevice1->show();
        ui->comboBoxDriver1->show();
        ui->labelDriverDeviceInput->show();
        ui->comboBoxDriverInput->show();
    } else {
        ui->labelDriverDevice0->hide();
        ui->comboBoxDriver0->hide();
        ui->labelDriverDevice1->hide();
        ui->comboBoxDriver1->hide();
        ui->labelDriverDeviceInput->hide();
        ui->comboBoxDriverInput->hide();
        HostInfoContainer* host = main->audio()->defaultHost() ? main->audio()->defaultHost() : nullptr;
        _displayHost0 = host;
        _displayHost1 = host;
        _displayHostInput = host;
    }

    bool inited = a->isInitialized();
    ui->groupBoxDevice0->setTitle(inited ? "Output Device 1" : "Output Device 1 (INITIALIZING...)");
    ui->groupBoxDevice1->setTitle(inited ? "Output Device 2" : "Output Device 2 (INITIALIZING...)");
    ui->groupBoxDeviceInput->setTitle(inited ? "Input Device" : "Input Device (INITIALIZING...)");
    ui->pushButtonPlay->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->pushButtonPause->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->pushButtonStop->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->pushButtonRefresh->setEnabled(inited);
    ui->checkBoxInput0->setEnabled(inited);
    ui->checkBoxInput1->setEnabled(inited);
    ui->muteButton->setEnabled(inited && a->activeInputs().size() > 0);
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
    } else if (!_inputObjectInited) {
        _inputObjectInited = true;
        main->audio()->inputObject()->setUpdateVisualizer(true);
        connect(main->audio()->inputObject(), &AudioObject::update, this, [&](float level) {
            ui->inputBar->setLevel(static_cast<qreal>(level));
        });
        updateMuteButton();
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
    QList<Device*>* displayedOutputs = new QList<Device*>();
    QList<Device*>* displayedInputs = new QList<Device*>();

    for (AudioDisplayContainer display : deviceDisplays) {
        QComboBox* drivers = display.drivers;
        QComboBox* devices = display.devices;
        drivers->clear();
        devices->clear();
        drivers->setEnabled(true);
        devices->setEnabled(true);
        display.deleteButton->setEnabled(true);

        QList<Device*>* displayedDevices = display.isInput ? displayedInputs : displayedOutputs;

        HostInfoContainer* displayHost = nullptr;
        bool notActiveDriver = false;

        // First add a single active device
        for (Device* dev : display.isInput ? a->activeInputs() : a->activeOutputs()) {
            // If the device hasn't already been added somewhere, and the display indexes match
            if (!displayedDevices->contains(dev) && display.deviceDisplayIndex == (display.isInput ? dev->indexes()->inputDisplayIndex : dev->indexes()->outputDisplayIndex)) {
                displayedDevices->append(dev);

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
                    ? a->defaultInput() && !displayedDevices->contains(a->defaultInput()) && displayHost == a->defaultInput()->host()
                    : a->defaultOutput() && !displayedDevices->contains(a->defaultOutput()) && displayHost == a->defaultOutput()->host()) {
                // If the device is active
                if (display.isInput
                        ? a->activeInputs().contains(a->defaultInput())
                        : a->activeOutputs().contains(a->defaultOutput())) {
                    // Make sure it's not being displayed by another device display
                    if (display.isInput
                            ? display.deviceDisplayIndex == a->defaultInput()->indexes()->inputDisplayIndex
                            : display.deviceDisplayIndex == a->defaultOutput()->indexes()->outputDisplayIndex) {
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
                if (!displayedDevices->contains(dev) && displayHost == dev->host() && dev != (display.isInput ? a->defaultInput() : a->defaultOutput())) {
                    // If the device is active
                    if (display.isInput ? a->activeInputs().contains(dev) : a->activeOutputs().contains(dev)) {
                        // Make sure it's not being displayed by another device display
                        if (display.deviceDisplayIndex == (display.isInput ? dev->indexes()->inputDisplayIndex : dev->indexes()->outputDisplayIndex)) {
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

    delete displayedOutputs;
    delete displayedInputs;
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

void DialogSettings::setOutputDeviceVolume(int value, int devDisplayIndex) {
    Device* dev = main->audio()->getActiveDisplayOutput(devDisplayIndex);
    if (dev) dev->setVolume(value);
}


void DialogSettings::on_sliderDevice0_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice0->maximum() && ui->spinBoxDevice0->value() > value)) {
        ui->spinBoxDevice0->setValue(value);
        setOutputDeviceVolume(value, 0);
        main->settings()->setValue(Main::OUTPUT_VOLUME0, value);
    }
}

void DialogSettings::on_spinBoxDevice0_valueChanged(int value)
{
    ui->sliderDevice0->setValue(value);
    setOutputDeviceVolume(value, 0);
    main->settings()->setValue(Main::OUTPUT_VOLUME0, value);
}

void DialogSettings::on_sliderDevice1_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderDevice1->maximum() && ui->spinBoxDevice1->value() > value)) {
        ui->spinBoxDevice1->setValue(value);
        setOutputDeviceVolume(value, 1);
        main->settings()->setValue(Main::OUTPUT_VOLUME1, value);
    }
}

void DialogSettings::on_spinBoxDevice1_valueChanged(int value)
{
    ui->sliderDevice1->setValue(value);
    setOutputDeviceVolume(value, 1);
    main->settings()->setValue(Main::OUTPUT_VOLUME1, value);
}

void DialogSettings::on_sliderInput_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderInput->maximum() && ui->spinBoxInput->value() > value)) {
        ui->spinBoxInput->setValue(value);
        if (_inputObjectInited) main->audio()->inputObject()->setVolumeInt(value);
        main->settings()->setValue(Main::INPUT_VOLUME0, value);
    }
}

void DialogSettings::on_spinBoxInput_valueChanged(int value)
{
    ui->sliderInput->setValue(value);
    if (_inputObjectInited) main->audio()->inputObject()->setVolumeInt(value);
    main->settings()->setValue(Main::INPUT_VOLUME0, value);
}

void DialogSettings::on_sliderTest_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderTest->maximum() && ui->spinBoxTest->value() > value)) {
        ui->spinBoxTest->setValue(value);
        audio.setVolumeInt(value);
        main->settings()->setValue(Main::TEST_VOLUME, value);
    }
}

void DialogSettings::on_spinBoxTest_valueChanged(int value)
{
    ui->sliderTest->setValue(value);
    audio.setVolumeInt(value);
    main->settings()->setValue(Main::TEST_VOLUME, value);
}

void DialogSettings::on_deleteButtonDevice0_clicked()
{
    outputRemoved(0, &_displayHost0);
}

void DialogSettings::on_deleteButtonDevice1_clicked()
{
    outputRemoved(1, &_displayHost1);
}

void DialogSettings::on_deleteButtonDeviceInput_clicked()
{
    inputRemoved(0, &_displayHostInput);
    if (_inputObjectInited) main->audio()->inputObject()->stop();
}

void DialogSettings::on_checkBoxInput0_clicked()
{
    bool enabled = ui->checkBoxInput0->isChecked();
    main->settings()->setValue(Main::INPUT_OUT0, enabled);
    if (_inputObjectInited) main->audio()->inputObject()->setOutput0(enabled);
}

void DialogSettings::on_checkBoxInput1_clicked()
{
    bool enabled = ui->checkBoxInput1->isChecked();
    main->settings()->setValue(Main::INPUT_OUT1, enabled);
    if (_inputObjectInited) main->audio()->inputObject()->setOutput1(enabled);
}

void DialogSettings::on_pushButtonTestFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load Audio File"), QString(), tr("(*.wav *.ogg *.flac)"));
    if (fn.isNull()) return;
    updateFileName(fn);
}

void DialogSettings::on_lineEditTestFile_textEdited(const QString &text)
{
    updateFileName(text);
}

void DialogSettings::updateFileName(QString fn) {
    // test to see if the file exists and is readable
    audio.setFile(fn);
    ui->lineEditTestFile->setText(fn);
    refreshDeviceSelection();
    main->settings()->setValue(Main::TEST_FILE, fn);
}

void DialogSettings::on_muteButton_clicked()
{
    if (!_inputObjectInited) return;
    // Toggles the mute button
    main->audio()->inputObject()->setMute(!main->audio()->inputObject()->isMuted());
    updateMuteButton();
}

void DialogSettings::updateMuteButton() {
    if (!_inputObjectInited) return;
    bool muted = main->audio()->inputObject()->isMuted();
    ui->muteButton->setIcon(QIcon(muted ? ":/icons/res/mic_off.png" : ":/icons/res/mic_on.png"));
    main->settings()->setValue(Main::INPUT_MUTED, muted);
}

void DialogSettings::on_checkBoxShowMuteButton_clicked()
{
    main->settings()->setValue(Main::SHOW_MUTE_BUTTON, ui->checkBoxShowMuteButton->isChecked());
}

void DialogSettings::on_checkBoxListDrivers_clicked(){

    bool enable = ui->checkBoxListDrivers->isChecked();
    bool hasDefaultHost = main->audio()->defaultHost();

    QString message;
    if (enable) {
        if (hasDefaultHost) {
            message = tr("The default device driver is already active.\n"
                         "Enabling other drivers may cause instability.\n"
                         "Are you sure you'd like to enable the other drivers?");
        } else {
            message = tr("There was no default driver detected. Enabling\n"
                         "other drivers may cause instability, but may help\n"
                         "to find a working device. Are you sure you'd\n"
                         "like to enable the other drivers?");
        }
    } else {
        if (hasDefaultHost) {
            message = tr("This will revert all devices back to the default\n"
                         "driver. Are you sure you'd like to disable the\n"
                         "other drivers?");
        } else {
            message = tr("This will revert all devices back to the default\n"
                         "driver, which wasn't detected. Are you sure you'd\n"
                         "like to disable the other drivers?");
        }
    }

    QMessageBox::StandardButton resBtn =
            QMessageBox::question(this, "Soundboard",
                                  message,
                                  QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                  QMessageBox::No);

    switch (resBtn) {
    case QMessageBox::Yes:
        main->settings()->setValue(Main::SHOW_DRIVERS, enable);

        if (enable) {
            // Show all drivers, and remove all displayHosts that don't have an
            // active driver, by disabling all the displayHosts
            _displayHost0 = nullptr;
            _displayHost1 = nullptr;
            _displayHostInput = nullptr;
        } else {
            // Revert to only default drivers
            if (hasDefaultHost) {
                for (Device* dev : main->audio()->activeOutputs())
                    // If the device isn't under the default host, remove it
                    if (dev->host() != main->audio()->defaultHost())
                        main->audio()->removeActiveDevice(dev, true, true);

                for (Device* dev : main->audio()->activeInputs())
                    // If the device isn't under the default host, remove it
                    if (dev->host() != main->audio()->defaultHost())
                        main->audio()->removeActiveDevice(dev, true, true);
            }

            main->audio()->updateSavedDevices();
        }

        refreshDeviceSelection();
        break;
    default:
        ui->checkBoxListDrivers->setChecked(!enable);
        return;
    }
}
