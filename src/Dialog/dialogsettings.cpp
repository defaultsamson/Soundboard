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
#include <QStyle>

// Allows storing the following types as QVariants
Q_DECLARE_METATYPE(HostInfoContainer*)
Q_DECLARE_METATYPE(Device*)

DialogSettings::DialogSettings(Main* _main) :
    DialogTestAudio(_main),
    ui(new Ui::DialogSettings),
    _main(_main)
{
    ui->setupUi(this);

    QObject::connect(this, SIGNAL(finished(int)), this, SLOT(onClose()));

    ui->tabWidget->setCurrentIndex(_main->settings()->value(Main::SETTINGS_TAB, 0).toInt());

#ifdef Q_OS_WIN
    ui->checkBoxDarkTheme->hide();
#endif
    ui->checkBoxDarkTheme->setChecked(_main->settings()->value(Main::DARK_THEME, false).toBool());
    ui->checkBoxShowMuteButton->setChecked(_main->settings()->value(Main::SHOW_MUTE_BUTTON, true).toBool());
    ui->checkBoxListDrivers->setChecked(_main->settings()->value(Main::SHOW_DRIVERS, false).toBool());
    ui->checkBoxWindowSize->setChecked(_main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool());
    // Restore the geometry, if it was saved
    if (_main->settings()->value(Main::REMEMBER_WINDOW_SIZES, true).toBool()) {
        if (_main->settings()->contains(Main::WINDOW_SETTINGS_WIDTH)) {
            resize(_main->settings()->value(Main::WINDOW_SETTINGS_WIDTH, 500).toInt(), _main->settings()->value(Main::WINDOW_SETTINGS_HEIGHT, 500).toInt());
        }
    }

    updateScrollAreaPalette();

    connect(ui->comboBoxDriverOutput0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::hostChangedOutput0);
    connect(ui->comboBoxDeviceOutput0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceChangedOutput0);
    connect(ui->comboBoxDriverOutput1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::hostChangedOutput1);
    connect(ui->comboBoxDeviceOutput1, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceChangedOutput1);
    connect(ui->comboBoxDriverInput0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::hostChangedInput0);
    connect(ui->comboBoxDeviceInput0, QOverload<int>::of(&QComboBox::activated), this, &DialogSettings::deviceChangedInput0);

    ui->volumeOutput0->setValue(_main->settings()->value(Main::OUTPUT_VOLUME0, 100).toInt());
    ui->volumeOutput1->setValue(_main->settings()->value(Main::OUTPUT_VOLUME1, 100).toInt());
    ui->volumeInput0->setValue(_main->settings()->value(Main::INPUT_VOLUME0, 100).toInt());
    connect(ui->volumeOutput0, &WidgetVolume::valueChanged, this, [this](int value){ setOutputDeviceVolume(value, 0); });
    connect(ui->volumeOutput1, &WidgetVolume::valueChanged, this, [this](int value){ setOutputDeviceVolume(value, 1); });
    connect(ui->volumeInput0, &WidgetVolume::valueChanged, this, [this](int value){ setInputDeviceVolume(value); });

    ui->checkBoxInput0Output0->setChecked(_main->settings()->value(Main::INPUT_OUT0, false).toBool());
    ui->checkBoxInput0Output1->setChecked(_main->settings()->value(Main::INPUT_OUT1, false).toBool());

    QString testFile = _main->settings()->value(Main::TEST_FILE, Main::DEFAULT_TEST_FILE).toString();
    audio.setFile(testFile);
    audio.setUpdateVisualizer(true);
    connect(&audio, &AudioObject::update, this, [this](float level) {
        ui->visualizerAudioFile->setLevel(static_cast<qreal>(level));
    });
    _main->audio()->registerAudio(&audio);
    if (audio.hasFile()) ui->lineEditAudioFile->setText(testFile);

    audio.setVolume(_main->settings()->value(Main::TEST_VOLUME, 100).toInt() / static_cast<float>(100));
    ui->volumeAudioFile->setValue(_main->settings()->value(Main::TEST_VOLUME, 100).toInt());
    // Connects the volume slider&box with the AudioObject
    connect(ui->volumeAudioFile, &WidgetVolume::valueChanged, &audio, &AudioObject::setVolumeInt);

    connect(ui->labelOutput0, &ClickableLabel::clicked, this, &DialogSettings::toggleOutput0);
    connect(ui->arrowOutput0, &ClickableLabel::clicked, this, &DialogSettings::toggleOutput0);
    connect(ui->labelOutput1, &ClickableLabel::clicked, this, &DialogSettings::toggleOutput1);
    connect(ui->arrowOutput1, &ClickableLabel::clicked, this, &DialogSettings::toggleOutput1);
    connect(ui->labelInput0, &ClickableLabel::clicked, this, &DialogSettings::toggleInput0);
    connect(ui->arrowInput0, &ClickableLabel::clicked, this, &DialogSettings::toggleInput0);
    connect(ui->labelAudioFile, &ClickableLabel::clicked, this, &DialogSettings::toggleAudioFile);
    connect(ui->arrowAudioFile, &ClickableLabel::clicked, this, &DialogSettings::toggleAudioFile);
    updateGroupBoxes();
    refreshDeviceSelection();

    _main->setAudioTestDialog(this);
    _main->disableKeybinds();

    // Load keybinds
    updateKeybindNaming();
    _main->hkEnableKeybinds->unreg();
    if (_main->hkEnableKeybinds->hasKey()) ui->keybindKeybindsEnable->setKey(_main->hkEnableKeybinds->key());
    _main->hkDisableKeybinds->unreg();
    if (_main->hkDisableKeybinds->hasKey()) ui->keybindKeybindsDisable->setKey(_main->hkDisableKeybinds->key());
    _main->hkStopSounds->unreg();
    if (_main->hkStopSounds->hasKey()) ui->keybindSoundsStop->setKey(_main->hkStopSounds->key());
    _main->hkPauseSounds->unreg();
    if (_main->hkPauseSounds->hasKey()) ui->keybindSoundsPause->setKey(_main->hkPauseSounds->key());
    _main->hkResumeSounds->unreg();
    if (_main->hkResumeSounds->hasKey()) ui->keybindSoundsResume->setKey(_main->hkResumeSounds->key());
    _main->hkMuteInput->unreg();
    if (_main->hkMuteInput->hasKey()) ui->keybindInput0Mute->setKey(_main->hkMuteInput->key());
    _main->hkUnmuteInput->unreg();
    if (_main->hkUnmuteInput->hasKey()) ui->keybindInput0Unmute->setKey(_main->hkUnmuteInput->key());
    _main->hkToggleMuteInput->unreg();
    if (_main->hkToggleMuteInput->hasKey()) ui->keybindInput0ToggleMute->setKey(_main->hkToggleMuteInput->key());

    ui->checkBoxNonNativeKeys->setChecked(_main->settings()->value(Main::NON_NATIVE_KEYNAMING, true).toBool());
}

void DialogSettings::updateKeybindNaming() {
    bool nonNative = _main->settings()->value(Main::NON_NATIVE_KEYNAMING, true).toBool();
    ui->keybindKeybindsEnable->updateKeyname(nonNative);
    ui->keybindKeybindsDisable->updateKeyname(nonNative);
    ui->keybindSoundsStop->updateKeyname(nonNative);
    ui->keybindSoundsPause->updateKeyname(nonNative);
    ui->keybindSoundsResume->updateKeyname(nonNative);
    ui->keybindInput0Mute->updateKeyname(nonNative);
    ui->keybindInput0Unmute->updateKeyname(nonNative);
    ui->keybindInput0ToggleMute->updateKeyname(nonNative);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::updateScrollAreaPalette() {
    QPalette pal = palette();
    // Hardcoded, there doesn't seem to be a way to not hardcode this, as the tabWidget doesn't seem to
    // have a QPalette::XXXXX value that correlates to its background. It seems to be QPalette:Button,
    // except that colour isn't accurate in light mode. QPalette::AlternateBase seems to be accurate
    // in dark mode.
    pal.setColor(QPalette::Window, _main->settings()->value(Main::DARK_THEME, false).toBool() ? QColor(66, 66, 66) : QColor(255, 255, 255));
    ui->scrollAreaAudio->setPalette(pal);
    ui->scrollAreaKeybinds->setPalette(pal);
    ui->scrollAreaOther->setPalette(pal);
}

void DialogSettings::onClose() {
    _main->audio()->unregisterAudio(&audio);
    _main->setAudioTestDialog(nullptr);
    _main->enableKeybinds();
    if (_inputObjectInited) _main->audio()->inputObject()->setUpdateVisualizer(false);

    _main->settings()->setValue(Main::TEST_VOLUME, ui->volumeAudioFile->value());

    // Save the geometry
    _main->settings()->setValue(Main::REMEMBER_WINDOW_SIZES, ui->checkBoxWindowSize->isChecked());
    _main->settings()->setValue(Main::WINDOW_SETTINGS_WIDTH, width());
    _main->settings()->setValue(Main::WINDOW_SETTINGS_HEIGHT, height());

    _main->updateMuteButton();

    // Save all global keybinds
    bool hasKey = ui->keybindKeybindsEnable->hasKey();
    quint32 key = ui->keybindKeybindsEnable->key();
    _main->settings()->setValue(Main::HK_ENABLE_KEYBINDS_HAS, hasKey);
    if (hasKey) {
        _main->hkEnableKeybinds->setKey(ui->keybindKeybindsEnable->key());
        _main->hkEnableKeybinds->reg();
        _main->settings()->setValue(Main::HK_ENABLE_KEYBINDS_KEY, key);
    } else {
        _main->hkEnableKeybinds->unSetKey();
    }


    hasKey = ui->keybindKeybindsDisable->hasKey();
    key = ui->keybindKeybindsDisable->key();
    _main->settings()->setValue(Main::HK_DISABLE_KEYBINDS_HAS, hasKey);
    if (hasKey) {
        _main->hkDisableKeybinds->setKey(ui->keybindKeybindsDisable->key());
        _main->hkDisableKeybinds->reg();
        _main->settings()->setValue(Main::HK_DISABLE_KEYBINDS_KEY, key);
    } else {
        _main->hkDisableKeybinds->unSetKey();
    }


    hasKey = ui->keybindSoundsStop->hasKey();
    key = ui->keybindSoundsStop->key();
    _main->settings()->setValue(Main::HK_STOP_SOUNDS_HAS, hasKey);
    if (hasKey) {
        _main->hkStopSounds->setKey(ui->keybindSoundsStop->key());
        _main->hkStopSounds->reg();
        _main->settings()->setValue(Main::HK_STOP_SOUNDS_KEY, key);
    } else {
        _main->hkStopSounds->unSetKey();
    }


    hasKey = ui->keybindSoundsPause->hasKey();
    key = ui->keybindSoundsPause->key();
    _main->settings()->setValue(Main::HK_PAUSE_SOUNDS_HAS, hasKey);
    if (hasKey) {
        _main->hkPauseSounds->setKey(ui->keybindSoundsPause->key());
        _main->hkPauseSounds->reg();
        _main->settings()->setValue(Main::HK_PAUSE_SOUNDS_KEY, key);
    } else {
        _main->hkPauseSounds->unSetKey();
    }


    hasKey = ui->keybindSoundsResume->hasKey();
    key = ui->keybindSoundsResume->key();
    _main->settings()->setValue(Main::HK_RESUME_SOUNDS_HAS, hasKey);
    if (hasKey) {
        _main->hkResumeSounds->setKey(ui->keybindSoundsResume->key());
        _main->hkResumeSounds->reg();
        _main->settings()->setValue(Main::HK_RESUME_SOUNDS_KEY, key);
    } else {
        _main->hkResumeSounds->unSetKey();
    }


    hasKey = ui->keybindInput0Mute->hasKey();
    key = ui->keybindInput0Mute->key();
    _main->settings()->setValue(Main::HK_MUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        _main->hkMuteInput->setKey(ui->keybindInput0Mute->key());
        _main->hkMuteInput->reg();
        _main->settings()->setValue(Main::HK_MUTE_INPUT_KEY, key);
    } else {
        _main->hkMuteInput->unSetKey();
    }


    hasKey = ui->keybindInput0Unmute->hasKey();
    key = ui->keybindInput0Unmute->key();
    _main->settings()->setValue(Main::HK_UNMUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        _main->hkUnmuteInput->setKey(ui->keybindInput0Unmute->key());
        _main->hkUnmuteInput->reg();
        _main->settings()->setValue(Main::HK_UNMUTE_INPUT_KEY, key);
    } else {
        _main->hkUnmuteInput->unSetKey();
    }


    hasKey = ui->keybindInput0ToggleMute->hasKey();
    key = ui->keybindInput0ToggleMute->key();
    _main->settings()->setValue(Main::HK_TOGGLE_MUTE_INPUT_HAS, hasKey);
    if (hasKey) {
        _main->hkToggleMuteInput->setKey(ui->keybindInput0ToggleMute->key());
        _main->hkToggleMuteInput->reg();
        _main->settings()->setValue(Main::HK_TOGGLE_MUTE_INPUT_KEY, key);
    } else {
        _main->hkToggleMuteInput->unSetKey();
    }
}

void DialogSettings::on_buttonBox_accepted()
{
    close();
}

void DialogSettings::on_buttonBox_rejected()
{
    close();
    // TODO make it revert all settings back
    // Possible give a message box warning to confirm discarding all settings
}

void DialogSettings::hostChangedOutput0(int index)
{
    QVariant qvar = ui->comboBoxDriverOutput0->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHost0 == host) return; // _displayHost0 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHost0 = host;
    refreshDeviceSelection();
}

void DialogSettings::deviceChangedOutput0(int index)
{
    outputChanged(ui->comboBoxDeviceOutput0, index, 0, &_displayHost0);
}

void DialogSettings::hostChangedOutput1(int index)
{
    QVariant qvar = ui->comboBoxDriverOutput1->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHost1 == host) return; // _displayHost1 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHost1 = host;
    refreshDeviceSelection();
}

void DialogSettings::deviceChangedOutput1(int index)
{
    outputChanged(ui->comboBoxDeviceOutput1, index, 1, &_displayHost1);
}

void DialogSettings::hostChangedInput0(int index)
{
    QVariant qvar = ui->comboBoxDriverInput0->itemData(index);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select backend..."

    HostInfoContainer* host = qvar.value<HostInfoContainer*>();
    if (_displayHostInput == host) return; // _displayHost1 starts out null, so doesn't work the first time, but prevents most unneccessary refreshes
    _displayHostInput = host;
    refreshDeviceSelection();
}

void DialogSettings::deviceChangedInput0(int index)
{
    inputChanged(ui->comboBoxDeviceInput0, index, 0, &_displayHostInput);
}

void DialogSettings::outputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    Device* dev = qvar.value<Device*>();
    if (_main->audio()->activeOutputs().contains(dev)) return; // If it's already active, ignore
    dev->indexes()->outputDisplayIndex = deviceDisplayIndex;

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    _main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);
    _main->audio()->addActiveOutput(dev);
    _main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}
void DialogSettings::inputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    QVariant qvar = selector->itemData(selectorIndex);
    if (qvar.type() == QVariant::Invalid) return; // This only happens when it says "Select device..."

    Device* dev = qvar.value<Device*>();
    if (_main->audio()->activeInputs().contains(dev)) return; // If it's already active, ignore
    dev->indexes()->inputDisplayIndex = deviceDisplayIndex;

    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    _main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    _main->audio()->addActiveInput(dev);
    _main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}

void DialogSettings::outputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    _main->audio()->removeActiveDisplayOutput(deviceDisplayIndex);
    _main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}
void DialogSettings::inputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost)
{
    *displayHost = nullptr; // The selected device now decides the display host, not this ptr, so set it null
    _main->audio()->removeActiveDisplayInput(deviceDisplayIndex);
    _main->audio()->updateSavedDevices();

    refreshDeviceSelection();
}

void DialogSettings::audioEngineInit() {
    refreshDeviceSelection();
}

QPixmap DialogSettings::modifyPixmap(QPixmap pixmap, bool dark, bool rotate) {
    if (!dark && !rotate) return pixmap;

    QImage tempImage = pixmap.toImage();
    if (dark) tempImage.invertPixels();
    if (rotate) tempImage = tempImage.transformed(QTransform().rotate(-90));

    return QPixmap::fromImage(std::move(tempImage));

    // From https://forum.qt.io/topic/77032/qicon-from-standardpixmap/10
    /*
    QIcon invertedIcon;
    for(auto singleMode : {QIcon::Normal,QIcon::Disabled,QIcon::Active,QIcon::Selected}){
        for(auto singleState : {QIcon::On,QIcon::Off}){
            const auto avalSize = icon.availableSizes(singleMode ,singleState );
            for(auto& singleSize : avalSize){
                QImage tempImage = icon.pixmap(singleSize,singleMode ,singleState).toImage();
                tempImage.invertPixels();
                invertedIcon.addPixmap(QPixmap::fromImage(std::move(tempImage)),singleMode,singleState);
            }
        }
    }
    return invertedIcon;
    */
}
void DialogSettings::toggleOutput0() {
    _main->settings()->setValue(Main::SHOW_SETTINGS_OUTPUT0, !_main->settings()->value(Main::SHOW_SETTINGS_OUTPUT0, true).toBool());
    updateGroupBoxes();
}
void DialogSettings::toggleOutput1() {
    _main->settings()->setValue(Main::SHOW_SETTINGS_OUTPUT1, !_main->settings()->value(Main::SHOW_SETTINGS_OUTPUT1, true).toBool());
    updateGroupBoxes();
}
void DialogSettings::toggleInput0() {
    _main->settings()->setValue(Main::SHOW_SETTINGS_INPUT0, !_main->settings()->value(Main::SHOW_SETTINGS_INPUT0, true).toBool());
    updateGroupBoxes();
}
void DialogSettings::toggleAudioFile() {
    _main->settings()->setValue(Main::SHOW_SETTINGS_AUDIO_FILE, !_main->settings()->value(Main::SHOW_SETTINGS_AUDIO_FILE, true).toBool());
    updateGroupBoxes();
}

void DialogSettings::updateGroupBoxes() {
    bool inited = _main->audio()->isInitialized();
    bool dark = _main->settings()->value(Main::DARK_THEME, false).toBool();

    bool show = _main->settings()->value(Main::SHOW_SETTINGS_OUTPUT0, true).toBool();
    QPixmap map = style()->standardPixmap(QStyle::SP_TitleBarUnshadeButton);
    Device* device = _main->audio()->getActiveDisplayOutput(0);
    ui->arrowOutput0->setPixmap(modifyPixmap(map, dark, !show));
    ui->labelOutput0->setText(inited
                              ? QString("Output Device 1") + (device ? QString(" (") + device->info()->name + ")" : "")
                              : "Output Device 1 (INITIALIZING...)");
    show ? ui->frameOutput0->show() : ui->frameOutput0->hide();

    show = _main->settings()->value(Main::SHOW_SETTINGS_OUTPUT1, true).toBool();
    map = style()->standardPixmap(QStyle::SP_TitleBarUnshadeButton);
    device = _main->audio()->getActiveDisplayOutput(1);
    ui->arrowOutput1->setPixmap(modifyPixmap(map, dark, !show));
    ui->labelOutput1->setText(inited
                              ? QString("Output Device 2") + (device ? QString(" (") + device->info()->name + ")" : "")
                              : "Output Device 2 (INITIALIZING...)");
    show ? ui->frameOutput1->show() : ui->frameOutput1->hide();

    show = _main->settings()->value(Main::SHOW_SETTINGS_INPUT0, true).toBool();
    map = style()->standardPixmap(QStyle::SP_TitleBarUnshadeButton);
    device = _main->audio()->getActiveDisplayInput(0);
    ui->arrowInput0->setPixmap(modifyPixmap(map, dark, !show));
    ui->labelInput0->setText(inited
                              ? QString("Input Device") + (device ? QString(" (") + device->info()->name + ")" : "")
                              : "Input Device (INITIALIZING...)");
    show ? ui->frameInput0->show() : ui->frameInput0->hide();

    show = _main->settings()->value(Main::SHOW_SETTINGS_AUDIO_FILE, true).toBool();
    map = style()->standardPixmap(QStyle::SP_TitleBarUnshadeButton);
    ui->arrowAudioFile->setPixmap(modifyPixmap(map, dark, !show));
    show ? ui->frameAudioFile->show() : ui->frameAudioFile->hide();
}

void DialogSettings::refreshDeviceSelection() {
    AudioEngine* a = _main->audio();

    bool showDrivers = _main->settings()->value(Main::SHOW_DRIVERS, false).toBool();

    ui->labelDriverOutput0->setVisible(showDrivers);
    ui->comboBoxDriverOutput0->setVisible(showDrivers);
    ui->labelDriverOutput1->setVisible(showDrivers);
    ui->comboBoxDriverOutput1->setVisible(showDrivers);
    ui->labelDriverInput0->setVisible(showDrivers);
    ui->comboBoxDeviceInput0->setVisible(showDrivers);

    // Revert all the dsplay hosts to the default host
    if (!showDrivers) {
        HostInfoContainer* host = _main->audio()->defaultHost() ? _main->audio()->defaultHost() : nullptr;
        _displayHost0 = host;
        _displayHost1 = host;
        _displayHostInput = host;
    }

    bool inited = a->isInitialized();
    updateGroupBoxes();
    ui->buttonPlay->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->buttonPause->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->buttonStop->setEnabled(inited && a->activeOutputs().size() > 0 && audio.hasFile());
    ui->buttonRefresh->setEnabled(inited);
    ui->checkBoxInput0Output0->setEnabled(inited);
    ui->checkBoxInput0Output1->setEnabled(inited);
    ui->buttonMuteInput0->setEnabled(inited && a->activeInputs().size() > 0);
    if (!inited) {
        ui->comboBoxDriverOutput0->clear();
        ui->comboBoxDriverOutput0->setEnabled(false);
        ui->comboBoxDeviceOutput0->clear();
        ui->comboBoxDeviceOutput0->setEnabled(false);
        ui->comboBoxDriverOutput1->clear();
        ui->comboBoxDriverOutput1->setEnabled(false);
        ui->comboBoxDeviceOutput1->clear();
        ui->comboBoxDeviceOutput1->setEnabled(false);
        ui->comboBoxDriverInput0->clear();
        ui->comboBoxDriverInput0->setEnabled(false);
        ui->comboBoxDeviceInput0->clear();
        ui->comboBoxDeviceInput0->setEnabled(false);
        ui->buttonDeleteOutput0->setEnabled(false);
        ui->buttonDeleteOutput1->setEnabled(false);
        ui->buttonDeleteInput0->setEnabled(false);
        return;
    } else if (!_inputObjectInited) {
        _inputObjectInited = true;
        _main->audio()->inputObject()->setUpdateVisualizer(true);
        connect(_main->audio()->inputObject(), &AudioObject::update, this, [&](float level) {
            ui->visualizerInput0->setLevel(static_cast<qreal>(level));
        });
        updateMuteButton();
    }

    /*
    if (a->getActiveDisplayOutput(0)) ui->spinBoxVolumeOutput0->setValue(a->getActiveDisplayOutput(0)->volumeInt());
    if (a->getActiveDisplayOutput(0)) ui->sliderVolumeOutput0->setValue(a->getActiveDisplayOutput(0)->volumeInt());
    if (a->getActiveDisplayOutput(1)) ui->spinBoxVolumeOutput1->setValue(a->getActiveDisplayOutput(1)->volumeInt());
    if (a->getActiveDisplayOutput(1)) ui->sliderVolumeOutput1->setValue(a->getActiveDisplayOutput(1)->volumeInt());
    if (a->getActiveDisplayInput(0)) ui->spinBoxVolumeInput0->setValue(a->getActiveDisplayInput(0)->volumeInt());
    if (a->getActiveDisplayInput(0)) ui->sliderVolumeInput0->setValue(a->getActiveDisplayInput(0)->volumeInt());
    */

    ui->checkBoxInput0Output0->setChecked(a->inputObject()->isActiveOutput0());
    ui->checkBoxInput0Output1->setChecked(a->inputObject()->isActiveOutput1());

    QList<AudioDisplayContainer> deviceDisplays;
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriverOutput0, ui->comboBoxDeviceOutput0, ui->buttonDeleteOutput0, _displayHost0, 0, false});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriverOutput1, ui->comboBoxDeviceOutput1, ui->buttonDeleteOutput1, _displayHost1, 1, false});
    deviceDisplays.append(AudioDisplayContainer{ui->comboBoxDriverInput0, ui->comboBoxDeviceInput0, ui->buttonDeleteInput0, _displayHostInput, 0, true});
    QList<Device*>* displayedOutputs = new QList<Device*>();
    QList<Device*>* displayedInputs = new QList<Device*>();

    for (AudioDisplayContainer display : deviceDisplays) {
        QComboBox* drivers = display.drivers;
        QComboBox* devices = display.devices;
        drivers->clear();
        devices->clear();
        drivers->setEnabled(true);
        devices->setEnabled(true);
        display.deleteButton->setEnabled(false);

        QList<Device*>* displayedDevices = display.isInput ? displayedInputs : displayedOutputs;

        HostInfoContainer* displayHost = nullptr;
        bool notActiveDriver = false;

        // First add a single active device
        for (Device* dev : display.isInput ? a->activeInputs() : a->activeOutputs()) {
            // If the device hasn't already been added somewhere, and the display indexes match
            if (!displayedDevices->contains(dev) && display.deviceDisplayIndex == (display.isInput ? dev->indexes()->inputDisplayIndex : dev->indexes()->outputDisplayIndex)) {
                displayedDevices->append(dev);
                display.deleteButton->setEnabled(true);

                // If the active device is in this displayIndex but not run by the displayed host
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

void DialogSettings::setOutputDeviceVolume(int value, int devDisplayIndex) {
    Device* dev = _main->audio()->getActiveDisplayOutput(devDisplayIndex);
    if (dev) dev->setVolumeInt(value);

    switch (devDisplayIndex) {
    case 0:
        _main->settings()->setValue(Main::OUTPUT_VOLUME0, value);
        break;
    case 1:
        _main->settings()->setValue(Main::OUTPUT_VOLUME1, value);
        break;
    }
}

void DialogSettings::setInputDeviceVolume(int value) {
    AudioObjectInput* dev = _main->audio()->inputObject();
    if (dev) dev->setVolumeInt(value);
    _main->settings()->setValue(Main::INPUT_VOLUME0, value);
}

void DialogSettings::updateFileName(QString fn) {
    // test to see if the file exists and is readable
    audio.setFile(fn);
    ui->lineEditAudioFile->setText(fn);
    refreshDeviceSelection();
    _main->settings()->setValue(Main::TEST_FILE, fn);
}

void DialogSettings::updateMuteButton() {
    if (!_inputObjectInited) return;
    bool muted = _main->audio()->inputObject()->isMuted();
    ui->buttonMuteInput0->setIcon(QIcon(muted ? ":/icons/res/mic_off.png" : ":/icons/res/mic_on.png"));
    _main->settings()->setValue(Main::INPUT_MUTED, muted);
}

void DialogSettings::on_checkBoxListDrivers_clicked(){

    bool enable = ui->checkBoxListDrivers->isChecked();
    bool hasDefaultHost = _main->audio()->defaultHost();

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
        _main->settings()->setValue(Main::SHOW_DRIVERS, enable);

        if (enable) {
            // Show all drivers, and remove all displayHosts that don't have an
            // active driver, by disabling all the displayHosts
            _displayHost0 = nullptr;
            _displayHost1 = nullptr;
            _displayHostInput = nullptr;
        } else {
            // Revert to only default drivers
            if (hasDefaultHost) {
                for (Device* dev : _main->audio()->activeOutputs())
                    // If the device isn't under the default host, remove it
                    if (dev->host() != _main->audio()->defaultHost())
                        _main->audio()->removeActiveDevice(dev, true, true);

                for (Device* dev : _main->audio()->activeInputs())
                    // If the device isn't under the default host, remove it
                    if (dev->host() != _main->audio()->defaultHost())
                        _main->audio()->removeActiveDevice(dev, true, true);
            }

            _main->audio()->updateSavedDevices();
        }

        refreshDeviceSelection();
        break;
    default:
        ui->checkBoxListDrivers->setChecked(!enable);
        return;
    }
}

void DialogSettings::on_buttonRefresh_clicked()
{
    _main->audio()->refreshDevices();
    _displayHost1 = nullptr; // Forces a refresh of the list of hosts and devices
    refreshDeviceSelection();
}

void DialogSettings::on_buttonPlay_clicked()
{
    audio.play();
}

void DialogSettings::on_buttonPause_clicked()
{
    audio.pause();
}

void DialogSettings::on_buttonStop_clicked()
{
    audio.stop();
}

/*
void DialogSettings::on_sliderVolumeAudioFile_valueChanged(int value)
{
    // Allow users to edit the number in the box past what the slider goes to
    if (!(value == ui->sliderVolumeAudioFile->maximum() && ui->spinBoxVolumeAudioFile->value() > value)) {
        ui->spinBoxVolumeAudioFile->setValue(value);
        audio.setVolumeInt(value);
        _main->settings()->setValue(Main::TEST_VOLUME, value);
    }
}

void DialogSettings::on_spinBoxVolumeAudioFile_valueChanged(int value)
{
    ui->sliderVolumeAudioFile->setValue(value);
    audio.setVolumeInt(value);
    _main->settings()->setValue(Main::TEST_VOLUME, value);
}*/

void DialogSettings::on_buttonDeleteOutput0_clicked()
{
    outputRemoved(0, &_displayHost0);
}

void DialogSettings::on_buttonDeleteOutput1_clicked()
{
    outputRemoved(1, &_displayHost1);
}

void DialogSettings::on_buttonDeleteInput0_clicked()
{
    inputRemoved(0, &_displayHostInput);
    if (_inputObjectInited) _main->audio()->inputObject()->stop();
}

void DialogSettings::on_checkBoxInput0Output0_clicked()
{
    bool enabled = ui->checkBoxInput0Output0->isChecked();
    _main->settings()->setValue(Main::INPUT_OUT0, enabled);
    if (_inputObjectInited) _main->audio()->inputObject()->setOutput0(enabled);
}

void DialogSettings::on_checkBoxInput0Output1_clicked()
{
    bool enabled = ui->checkBoxInput0Output1->isChecked();
    _main->settings()->setValue(Main::INPUT_OUT1, enabled);
    if (_inputObjectInited) _main->audio()->inputObject()->setOutput1(enabled);
}

void DialogSettings::on_lineEditAudioFile_textEdited(const QString &text)
{
    updateFileName(text);
}

void DialogSettings::on_buttonMuteInput0_clicked()
{
    if (!_inputObjectInited) return;
    // Toggles the mute button
    _main->audio()->inputObject()->setMute(!_main->audio()->inputObject()->isMuted());
    updateMuteButton();
}

void DialogSettings::on_checkBoxShowMuteButton_clicked()
{
    _main->settings()->setValue(Main::SHOW_MUTE_BUTTON, ui->checkBoxShowMuteButton->isChecked());
}

void DialogSettings::on_buttonAudioFile_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Load Audio File"), QString(), tr("(*.wav *.ogg *.flac)"));
    if (fn.isNull()) return;
    updateFileName(fn);
}

void DialogSettings::on_tabWidget_currentChanged(int index)
{
    _main->settings()->setValue(Main::SETTINGS_TAB, index);
}

void DialogSettings::on_checkBoxDarkTheme_clicked()
{
    _main->setDarkTheme(ui->checkBoxDarkTheme->isChecked());
    updateScrollAreaPalette();
    updateGroupBoxes();
}

void DialogSettings::on_checkBoxNonNativeKeys_clicked()
{
    _main->settings()->setValue(Main::NON_NATIVE_KEYNAMING, ui->checkBoxNonNativeKeys->isChecked());
    updateKeybindNaming();
}
