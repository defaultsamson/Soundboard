#include "mainapp.h"
#include "ui_mainapp.h"

#include "Audio/audioengine.h"
#include "Widget/listitemboard.h"
#include "Widget/listitemsound.h"
#include "Dialog/dialogboard.h"
#include "Dialog/dialogsound.h"
#include "Dialog/dialogsettings.h"
#include "Dialog/dialogtestaudio.h"
#include "Hotkey/hotkeytrigger.h"

#include <QApplication>
#include <singleapplication.h>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QSettings>
#include <QModelIndex>
#include <QObject>
#include <QAbstractItemModel>
#include <QThread>
#include <QCloseEvent>
#include <QMessageBox>
#include <QGroupBox>

class MyThread : public QThread
{
public:
    MyThread(Main* _main);
protected:
    void run();
private:
    Main* _main;
};        

MyThread::MyThread(Main* _main) : _main(_main) {
    QObject::connect(this, SIGNAL(finished()), _main, SLOT(updateMuteButton()));
}

void MyThread::run()
{
    // Set up testing audio
    QFile testFile(Main::DEFAULT_TEST_FILE);
    if (!testFile.exists()) {
        QFile::copy(":/audio/res/test.ogg", Main::DEFAULT_TEST_FILE);
    }

    _main->audio()->init();
    if (_main->getAudioTestDialog()) _main->getAudioTestDialog()->audioEngineInit();
}

void Main::setAudioTestDialog(DialogTestAudio *s) {
    _audioTestDialog = s;
}
DialogTestAudio *Main::getAudioTestDialog() {
    return _audioTestDialog;
}

int main(int argc, char *argv[])
{
    // From https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection
    // RunGuard guard("soundboard_totally_random_but_unique_key");
    // if (!guard.tryToRun()) return 0;

    SingleApplication a(argc, argv);
    if (a.isSecondary()) {
        a.exit(0);
        return a.exec();
    }

    Main w;
    w.show();
    w.restoreSizes();

    // Bring attention to the window if a new instance is started
    QObject::connect(
        &a,
        &SingleApplication::instanceStarted,
        &w,
        [&]{ w.raise(); w.activateWindow(); w.showNormal(); }
    );

    // Initializes audio engine on separate thread so that the UI starts super fast
    MyThread t(&w);
    t.start();

    return a.exec();
}

QString Main::DEFAULT_DIR = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Soundboard/";
QString Main::SETTINGS_FILE = "settings.ini";
QString Main::DEFAULT_SOUNDBOARD = "default.json";
QString Main::DEFAULT_TEST_FILE = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/test.ogg";

QString Main::DARK_THEME = "dark_theme";
bool Main::DARK_THEME_DEFAULT = false;
QString Main::ACTIVE_BOARD = "active_board";
QString Main::ACTIVE_BOARD_DEFAULT = "";
QString Main::HAS_ACTIVE_BOARD = "has_active_board";
bool Main::HAS_ACTIVE_BOARD_DEFAULT = false;
QString Main::SETTINGS_TAB = "settings_tab";

QString Main::HK_ENABLE_KEYBINDS_HAS = "hk_enable_keybinds_has";
QString Main::HK_ENABLE_KEYBINDS_KEY = "hk_enable_keybinds_key";
QString Main::HK_DISABLE_KEYBINDS_HAS = "hk_disable_keybinds_has";
QString Main::HK_DISABLE_KEYBINDS_KEY = "hk_disable_keybinds_key";
QString Main::HK_STOP_SOUNDS_HAS = "hk_stop_sounds_has";
QString Main::HK_STOP_SOUNDS_KEY = "hk_stop_sounds_key";
QString Main::HK_PAUSE_SOUNDS_HAS = "hk_pause_sounds_has";
QString Main::HK_PAUSE_SOUNDS_KEY = "hk_pause_sounds_key";
QString Main::HK_RESUME_SOUNDS_HAS = "hk_resume_sounds_has";
QString Main::HK_RESUME_SOUNDS_KEY = "hk_resume_sounds_key";
QString Main::HK_MUTE_INPUT_HAS = "hk_mute_input_has";
QString Main::HK_MUTE_INPUT_KEY = "hk_mute_input_key";
QString Main::HK_UNMUTE_INPUT_HAS = "hk_unmute_input_has";
QString Main::HK_UNMUTE_INPUT_KEY = "hk_unmute_input_key";
QString Main::HK_TOGGLE_MUTE_INPUT_HAS = "hk_toggle_mute_input_has";
QString Main::HK_TOGGLE_MUTE_INPUT_KEY = "hk_toggle_mute_input_key";

QString Main::REMEMBER_WINDOW_SIZES = "remember_window_sizes";
QString Main::WINDOW_MAIN_GEOMETRY = "window__main_geometry";
QString Main::WINDOW_MAIN_SPLITTER0 = "window__main_slider0";
QString Main::WINDOW_MAIN_SPLITTER1 = "window__main_slider1";
QString Main::WINDOW_SETTINGS_WIDTH = "window_settings_width";
QString Main::WINDOW_SETTINGS_HEIGHT = "window_settings_height";
QString Main::WINDOW_BOARD_WIDTH = "window_board_width";
QString Main::WINDOW_BOARD_HEIGHT = "window_board_height";
QString Main::WINDOW_SOUND_WIDTH = "window_sound_width";
QString Main::WINDOW_SOUND_HEIGHT = "window_sound_height";
QString Main::SHOW_SETTINGS_OUTPUT0 = "show_settings_output0";
QString Main::SHOW_SETTINGS_OUTPUT1 = "show_settings_output1";
QString Main::SHOW_SETTINGS_INPUT0 = "show_settings_input0";

QString Main::EXPLICIT_NO_OUTPUT_DEVICES = "explicit_no_outputs";
QString Main::EXPLICIT_NO_INPUT_DEVICES = "explicit_no_inputs";
QString Main::OUTPUT_INDEX0 = "output_index0";
QString Main::OUTPUT_INDEX1 = "output_index1";
QString Main::INPUT_INDEX0 = "input_index0";
QString Main::OUTPUT_VOLUME0 = "output_volume0";
QString Main::OUTPUT_VOLUME1 = "output_volume1";
QString Main::INPUT_VOLUME0 = "input_volume0";
QString Main::TEST_VOLUME = "test_volume";
QString Main::INPUT_OUT0 = "input_out0";
QString Main::INPUT_OUT1 = "input_out1";
QString Main::INPUT_MUTED = "input_muted";
QString Main::SHOW_MUTE_BUTTON = "show_mute_button";
QString Main::SHOW_DRIVERS = "show_drivers";
QString Main::TEST_FILE = "test_file";

Main::Main(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainApp),
    _audio(new AudioEngine(this)),
    _settings(new QSettings(DEFAULT_DIR + SETTINGS_FILE, QSettings::IniFormat)),
    defaultPalette(qApp->palette())
{
    ui->setupUi(this);

    // Sets up the onSoundMoved event
    QObject::connect(ui->listSounds->model(), &QAbstractItemModel::rowsMoved, this, &Main::onSoundMoved);

    // Sets up right click context menus for the Boards and Sounds lists
    ui->listSounds->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->listSounds, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuSound(QPoint)));
    ui->listBoards->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(ui->listBoards, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuBoard(QPoint)));

    // Colours from https://github.com/Jorgen-VikingGod/Qt-Frameless-Window-DarkStyle/blob/master/DarkStyle.cpp
    // (mirror): https://stackoverflow.com/a/45634644/1902411
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Base, QColor(42, 42, 42));
    darkPalette.setColor(QPalette::AlternateBase, QColor(66, 66, 66));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Dark, QColor(35, 35, 35));
    darkPalette.setColor(QPalette::Shadow, QColor(20, 20, 20));
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(127, 127, 127));

    // Light, Midlight, Mid, NoRole, PlaceholderText

    setDarkTheme(settings()->value(DARK_THEME, DARK_THEME_DEFAULT).toBool());

    // Load the previously active board
    if (settings()->value(HAS_ACTIVE_BOARD, HAS_ACTIVE_BOARD_DEFAULT).toBool()) {
        load(settings()->value(ACTIVE_BOARD, ACTIVE_BOARD_DEFAULT).toString());
    }

    connect(ui->listSounds, &QListWidget::currentRowChanged, this, &Main::rowChanged);

    // Loads global keybinds & their behaviour
    hkEnableKeybinds = new HotkeyTrigger(this, [this]{ enableKeybinds(); });
    if (settings()->value(HK_ENABLE_KEYBINDS_HAS, false).toBool()) {
        hkEnableKeybinds->setKey(static_cast<quint32>(settings()->value(HK_ENABLE_KEYBINDS_KEY, 0).toInt()));
        hkEnableKeybinds->reg();
    }
    hkDisableKeybinds = new HotkeyTrigger(this, [this]{ disableKeybinds(); });
    if (settings()->value(HK_DISABLE_KEYBINDS_HAS, false).toBool()) {
        hkDisableKeybinds->setKey(static_cast<quint32>(settings()->value(HK_DISABLE_KEYBINDS_KEY, 0).toInt()));
        hkDisableKeybinds->reg();
    }
    hkStopSounds = new HotkeyTrigger(this, [this]{ for (auto sound : _audio->audioRegistry()) sound->stop(); });
    if (settings()->value(HK_STOP_SOUNDS_HAS, false).toBool()) {
        hkStopSounds->setKey(static_cast<quint32>(settings()->value(HK_STOP_SOUNDS_KEY, 0).toInt()));
        hkStopSounds->reg();
    }
    hkPauseSounds = new HotkeyTrigger(this, [this]{ for (auto sound : _audio->audioRegistry()) sound->pause(); });
    if (settings()->value(HK_PAUSE_SOUNDS_HAS, false).toBool()) {
        hkPauseSounds->setKey(static_cast<quint32>(settings()->value(HK_PAUSE_SOUNDS_KEY, 0).toInt()));
        hkPauseSounds->reg();
    }
    hkResumeSounds = new HotkeyTrigger(this, [this]{ for (auto sound : _audio->audioRegistry()) if (sound->isPaused()) sound->play(); });
    if (settings()->value(HK_RESUME_SOUNDS_HAS, false).toBool()) {
        hkResumeSounds->setKey(static_cast<quint32>(settings()->value(HK_RESUME_SOUNDS_KEY, 0).toInt()));
        hkResumeSounds->reg();
    }
    hkMuteInput = new HotkeyTrigger(this, [this]{ if (_audio->inputObject()) _audio->inputObject()->setMute(true); updateMuteButton(); });
    if (settings()->value(HK_MUTE_INPUT_HAS, false).toBool()) {
        hkMuteInput->setKey(static_cast<quint32>(settings()->value(HK_MUTE_INPUT_KEY, 0).toInt()));
        hkMuteInput->reg();
    }
    hkUnmuteInput = new HotkeyTrigger(this, [this]{ if (_audio->inputObject()) _audio->inputObject()->setMute(false); updateMuteButton(); });
    if (settings()->value(HK_UNMUTE_INPUT_HAS, false).toBool()) {
        hkUnmuteInput->setKey(static_cast<quint32>(settings()->value(HK_UNMUTE_INPUT_KEY, 0).toInt()));
        hkUnmuteInput->reg();
    }
    hkToggleMuteInput = new HotkeyTrigger(this, [this]{ if (_audio->inputObject()) _audio->inputObject()->setMute(!_audio->inputObject()->isMuted()); updateMuteButton(); });
    if (settings()->value(HK_TOGGLE_MUTE_INPUT_HAS, false).toBool()) {
        hkToggleMuteInput->setKey(static_cast<quint32>(settings()->value(HK_TOGGLE_MUTE_INPUT_KEY, 0).toInt()));
        hkToggleMuteInput->reg();
    }

    updateMuteButton();
}

Main::~Main()
{
    clear();
    delete ui;
    delete _audio;
    delete _settings;

    delete hkEnableKeybinds;
    delete hkDisableKeybinds;
    delete hkStopSounds;
    delete hkPauseSounds;
    delete hkResumeSounds;
    delete hkMuteInput;
    delete hkUnmuteInput;
    delete hkToggleMuteInput;
}

// https://stackoverflow.com/questions/31383519/qt-rightclick-on-qlistwidget-opens-contextmenu-and-delete-item
void Main::contextMenuBoard(const QPoint &pos) {
    int row = ui->listBoards->indexAt(pos).row();
    QMenu myMenu;
    // If the mouse is hovering over something
    if (row >= 0) {
        // Select the current board (have to do this or else Qt is too slow when switching items really fast)
        setCurrentBoard(row);
        myMenu.addAction("Edit", this, [&]() { editBoard(); });
        myMenu.addAction("Delete",  this, [&]() { removeBoard(currentBoard); });
    } else {
        myMenu.addAction("Create", this, [&]() { addBoard(); });
    }

    // Handle global position
    QPoint globalPos = ui->listBoards->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1); // makes right clicking feel more natural
    globalPos.setY(globalPos.y() + 1);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

void Main::contextMenuSound(const QPoint &pos) {
    if (!currentBoard) return;
    int row = ui->listSounds->indexAt(pos).row();
    QMenu myMenu;
    // If the mouse is hovering over something
    if (row >= 0) {
        // Select the current board (have to do this or else Qt is too slow when switching items really fast)
        ui->listSounds->setCurrentRow(row);
        myMenu.addAction("Edit", this, [&]() { editSound(); });
        myMenu.addAction("Delete",  this, [&]() { removeSound(currentSound()); });
    } else {
        myMenu.addAction("Create", this, [&]() { addSound(); });
    }

    // Handle global position
    QPoint globalPos = ui->listSounds->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1); // makes right clicking feel more natural
    globalPos.setY(globalPos.y() + 1);

    // Show context menu at handling position
    myMenu.exec(globalPos);
}

// ******************* BGN MENU ACTIONS *******************

// New
void Main::on_actionNew_triggered()
{
    if (_changed) {
        QMessageBox::StandardButton resBtn = Main::unsavedChangedDialogue();
        switch (resBtn) {
        case QMessageBox::Save:
            save();
        case QMessageBox::Discard:
            break;
        default:
            return;
        }
    }

    clear();
    hasFile = false;
    fileName = "";
    updateTitle();
}

// Open
void Main::on_actionOpen_triggered()
{
    if (_changed) {
        QMessageBox::StandardButton resBtn = Main::unsavedChangedDialogue();
        switch (resBtn) {
        case QMessageBox::Save:
            save();
        case QMessageBox::Discard:
            break;
        default:
            return;
        }
    }
    load();
}

// Save
void Main::on_actionSave_triggered()
{
    save();
}

// Save As
void Main::on_actionSaveAs_triggered()
{
    save(true);
}

// Exit program
void Main::on_actionExit_triggered()
{
    if (_changed) {
        QMessageBox::StandardButton resBtn = Main::unsavedChangedDialogue();
        switch (resBtn) {
        case QMessageBox::Save:
            save();
        case QMessageBox::Discard:
            break;
        default:
            return;
        }
    }
    QApplication::quit();
}

// Settings (e.g. Sound set)
void Main::on_actionSettings_triggered()
{
    DialogSettings w(this);
    w.exec();
}

// Wiki Page
void Main::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/wiki", QUrl::TolerantMode));
}

// Github Page
void Main::on_actionGitHub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard", QUrl::TolerantMode));
}

// Update
void Main::on_actionUpdate_triggered()
{
    // TODO check properly instead of just linking to the download page
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/releases", QUrl::TolerantMode));
}

// ******************* END MENU ACTIONS *******************
// ******************* BGN LIST ACTIONS *******************

// Board item changed
void Main::on_listBoards_currentItemChanged(QListWidgetItem* current, QListWidgetItem* /* previous */)
{
    setCurrentBoard(static_cast<ListItemBoard*>(current));
}

// Board item double clicked, enter edit mode
void Main::on_listBoards_itemActivated(QListWidgetItem* item)
{
    editBoard(static_cast<ListItemBoard*>(item));
}

// Sound item double clicked, enter edit mode
void Main::on_listSounds_itemActivated(QListWidgetItem* item)
{
    editSound(static_cast<ListItemSound*>(item));
}

void Main::onSoundMoved(const QModelIndex &/*parent*/, int start, int /*end*/, const QModelIndex &/*destination*/, int row) {
    // We can ignore "end" since we know that there will always only be one item being moved
    if (row > start) row -= 1; // This is because the row variable refers to the index of the space inbetween each item index, this converts it to the item index itself
    // std::cout << "Moved from " << start << " to " << row << std::endl;
    if (start >= 0 && row >= 0) currentBoard->moveSound(static_cast<size_t>(start), static_cast<size_t>(row));
}

void Main::on_listSounds_itemClicked(QListWidgetItem* /*item*/) {
    ui->buttonEditSound->setEnabled(true);
    ui->buttonRemoveSound->setEnabled(true);
    ui->buttonPlay->setEnabled(true);
    ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
}

void Main::on_listBoards_itemClicked(QListWidgetItem* /*item*/) {
    ui->buttonEditBoard->setEnabled(true);
    ui->buttonRemoveBoard->setEnabled(true);
}

// ******************* END LIST ACTIONS *******************
// ******************* BGN BUTTON ACTIONS *******************

// Add board item
void Main::on_buttonAddBoard_clicked()
{
    addBoard();
}

// Remove board item
void Main::on_buttonRemoveBoard_clicked()
{
    removeBoard(currentBoard);
}

void Main::on_buttonEditBoard_clicked()
{
    editBoard();
}

// Add sound item
void Main::on_buttonAddSound_clicked()
{
    addSound();
}

// Remove sound item
void Main::on_buttonRemoveSound_clicked()
{
    removeSound(currentSound());
}

void Main::on_buttonEditSound_clicked()
{
    editSound();
}

// ******************* END BUTTON ACTIONS *******************
// ******************* BGN BOARD FUNCTIONS *******************

void Main::addBoard() {
    ListItemBoard* board = new ListItemBoard(this);
    ui->listBoards->addItem(board);
    setCurrentBoard(board);
    editBoard(board, true);
}

void Main::addSound() {
    if (!currentBoard) return;
    ListItemSound* sound = new ListItemSound(this, currentBoard);
    currentBoard->addSound(sound);
    displayBoard(currentBoard);
    setCurrentSound(sound);
    ui->buttonEditSound->setEnabled(true);
    ui->buttonRemoveSound->setEnabled(true);
    ui->buttonPlay->setEnabled(true);
    ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    editSound(sound, true);
}

void Main::removeBoard(ListItemBoard* board, bool wasNew) {
    removeBoard(ui->listBoards->row(board), wasNew);
}

void Main::removeBoard(int row, bool wasNew) {
    if (row < 0 || row >= ui->listBoards->count()) return;
    delete ui->listBoards->takeItem(row);
    if (!wasNew) setChanged();
}

ListItemBoard* Main::getBoard(int row) {
    return row >= 0 ? static_cast<ListItemBoard*>(ui->listBoards->item(row)) : nullptr;
}

void Main::editBoard(bool createNew) {
    editBoard(currentBoard, createNew);
}

void Main::editBoard(ListItemBoard* board, bool createNew) {
    if (!board) return;
    DialogBoard w(this, board, createNew);
    w.exec();
}

void Main::displayBoard(ListItemBoard* board) {
    // TODO not do if settings don't want to update visuals
    if (board != ui->listBoards->currentItem()) {
        ui->listBoards->setCurrentItem(board);
    }
    board->populateList(ui->listSounds);
    bool isSoundSelected = ui->listSounds->currentRow() >= 0;
    ui->buttonAddSound->setEnabled(true);
    ui->buttonEditSound->setEnabled(isSoundSelected);
    ui->buttonRemoveSound->setEnabled(isSoundSelected);
    ui->buttonPlay->setEnabled(isSoundSelected);
    ui->buttonPause->setEnabled(isSoundSelected);
    ui->buttonStop->setEnabled(isSoundSelected);
}

void Main::setCurrentBoard(int row) {
    if (row < 0 || row >= ui->listBoards->count()) return;
    setCurrentBoard(getBoard(row));
}

void Main::setCurrentBoard(ListItemBoard* board) {
    if (currentBoard == board) return;
    if (currentBoard) {
        currentBoard->unreg(false);
    }
    currentBoard = board;
    if (currentBoard) {
        currentBoard->reg(false, true);
        displayBoard(currentBoard);

        ui->buttonEditBoard->setEnabled(true);
        ui->buttonRemoveBoard->setEnabled(true);
    } else {

        ui->buttonEditBoard->setEnabled(false);
        ui->buttonRemoveBoard->setEnabled(false);

        ui->buttonAddSound->setEnabled(false);
        ui->buttonEditSound->setEnabled(false);
        ui->buttonRemoveSound->setEnabled(false);
        ui->buttonPlay->setEnabled(false);
        ui->buttonPause->setEnabled(false);
        ui->buttonStop->setEnabled(false);
    }
}

void Main::setCurrentSound(int row) {
    if (row < 0 || row >= ui->listSounds->count()) return;
    setCurrentSound(getSound(row));
}

void Main::setCurrentSound(ListItemSound* sound) {
    // TODO don't do display if the settings say not to
    ui->listSounds->setCurrentItem(sound);
    // TODO play audio
}

void Main::removeSound(ListItemSound* sound, bool wasNew) {
    removeSound(ui->listSounds->row(sound), wasNew);
}

void Main::removeSound(int row, bool wasNew) {
    if (row < 0 || row >= ui->listSounds->count()) return;
    delete ui->listSounds->takeItem(row);
    displayBoard(currentBoard);
    if (!wasNew) setChanged();
}

ListItemSound* Main::getSound(int row) {
    return row >= 0 ? static_cast<ListItemSound*>(ui->listSounds->item(row)) : nullptr;
}

void Main::editSound(bool createNew) {
    editSound(currentSound(), createNew);
}

void Main::editSound(ListItemSound* sound, bool createNew) {
    if (!sound) return;
    DialogSound w(this, sound, createNew);
    w.exec();
}

void Main::clear() {
    for (int i = ui->listSounds->count() - 1; i >= 0; --i) ui->listSounds->takeItem(i);
    for (int i = ui->listBoards->count() - 1; i >= 0; --i) delete ui->listBoards->takeItem(i);
}

ListItemSound* Main::currentSound() {
    return getSound(ui->listSounds->currentRow());
}

// ******************* END BOARD FUNCTIONS *******************
// ******************* BGN FILE FUNCTIONS *******************

void Main::load() {
    load(QFileDialog::getOpenFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)")));
}

void Main::load(const QString fn) {
    QFile file(fn);

    qDebug() << "Loading File: " << fn;
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        // TODO inform user the file failed to load
        settings()->setValue(HAS_ACTIVE_BOARD, false);
        return;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    QJsonObject obj = loadDoc.object();
    QJsonArray arr = obj["boards"].toArray();

    clear();

    // Loads all the boards
    for (int i = 0; i < arr.size(); ++i) {
        ListItemBoard* board = new ListItemBoard(this);
        board->load(arr[i].toObject());
        ui->listBoards->addItem(board);
        // Set the first board as the current board
        if (i == 0) {
            setCurrentBoard(board);
        }
    }
    enableKeybinds();

    // setCurrentBoard(obj["currentboard"].toInt());
    // setCurrentSound(obj["currentsound"].toInt());

    hasFile = true;
    fileName = fn;
    settings()->setValue(HAS_ACTIVE_BOARD, true);
    settings()->setValue(ACTIVE_BOARD, fileName);
    updateTitle();
}

void Main::save(bool saveAs) {
    QString tempFileName;
    if (saveAs) { // Let the user choose a save file
        tempFileName = QFileDialog::getSaveFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)"));
    } else if (!hasFile) { // Use default
        tempFileName = DEFAULT_DIR + DEFAULT_SOUNDBOARD;
    } else {
        tempFileName = fileName;
    }

    QFile file(tempFileName);

    // If overwriting an existing board without knowing it
    if (file.exists() && !hasFile) {
        QString title = "Overwrite board?\n";
        title += tempFileName;
        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Soundboard",
                                                                    tr(title.toStdString().c_str()),
                                                                    QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                    QMessageBox::No);
        switch (resBtn) {
        case QMessageBox::Yes:
            break;
        default:
            return;
        }
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't write to save file.");
        // TODO inform user the file failed to save
        settings()->setValue(HAS_ACTIVE_BOARD, false);
        return;
    }

    QJsonObject json;

    // Creates an array of boards
    QJsonArray boards;
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        QJsonObject b;
        getBoard(i)->save(b);
        boards.append(b);
    }

    json["boards"] = boards;
    // json["currentboard"] = currentBoard ? ui->listBoards->currentRow() : -1;
    // json["currentsound"] = currentSound() ? ui->listSounds->currentRow() : -1;

    // Turns the object into text and saves it
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());

    hasFile = true;
    fileName = tempFileName;
    setChanged(false);
    updateTitle();
    settings()->setValue(HAS_ACTIVE_BOARD, true);
    settings()->setValue(ACTIVE_BOARD, fileName);
}

// ******************* END FILE FUNCTIONS *******************

void Main::enableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        getBoard(i)->reg(true, false);
    }
    if (currentBoard) currentBoard->reg(false, true);
}

void Main::disableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        getBoard(i)->unreg(true);
    }
}

void Main::setDarkTheme(bool set) {
    qApp->setPalette(set ? darkPalette : defaultPalette);
    settings()->setValue(DARK_THEME, set);
}

AudioEngine *Main::audio() {
    return _audio;
}

QSettings *Main::settings() {
    return _settings;
}

QMessageBox::StandardButton Main::unsavedChangedDialogue() {
    return QMessageBox::question( this, "Soundboard",
                                  tr("Handle unsaved changes?"),
                                  QMessageBox::Cancel | QMessageBox::Discard | QMessageBox::Save,
                                  QMessageBox::Save);
}

void Main::closeEvent (QCloseEvent* event)
{
    if (_changed) {
        QMessageBox::StandardButton resBtn = Main::unsavedChangedDialogue();
        switch (resBtn) {
        case QMessageBox::Save:
            save();
        case QMessageBox::Discard:
            event->accept();
            break;
        default:
            event->ignore();
            return;
        }
    } else {
        event->accept();
    }

    // Save the _main window sizes
    settings()->setValue(WINDOW_MAIN_GEOMETRY, saveGeometry());
    QList<int> sizes = ui->splitter->sizes();
    settings()->setValue(WINDOW_MAIN_SPLITTER0, sizes.at(0));
    settings()->setValue(WINDOW_MAIN_SPLITTER1, sizes.at(1));
    settings()->setValue(INPUT_MUTED, _audio->inputObject()->isMuted());
}

void Main::setChanged(bool changed) {
    _changed = changed;
    updateTitle();
}

void Main::updateTitle() {
    QString title = "Soundboard";
    if (hasFile) {
        QStringList splitted;
        // This is to differentiate by each platform's file system
        if (fileName.contains("\\")) splitted = fileName.split("\\");
        else splitted = fileName.split("/");

        title += " (" + splitted.last() + ")";
    }
    if (_changed) title += "*";
    setWindowTitle(title);
}

void Main::on_buttonPlay_clicked()
{
    if (currentSound()) currentSound()->audio()->play();
}

void Main::on_buttonPause_clicked()
{
    if (currentSound()) currentSound()->audio()->pause();
}

void Main::on_buttonStop_clicked()
{
    if (currentSound()) currentSound()->audio()->stop();
}

void Main::rowChanged(int row) {
    bool soundSelected = row >= 0;
    ui->buttonEditSound->setEnabled(soundSelected);
    ui->buttonRemoveSound->setEnabled(soundSelected);
    ui->buttonPlay->setEnabled(soundSelected);
    ui->buttonPause->setEnabled(soundSelected);
    ui->buttonStop->setEnabled(soundSelected);
}

void Main::on_splitter_splitterMoved(int pos, int /*index*/)
{
    updateButtonBar(pos);
}

void Main::updateButtonBar() {
    updateButtonBar(ui->splitter->sizes().at(0));
}

void Main::updateButtonBar(int pos) {

    int width = ui->splitter->width();
    int handleWidth = ui->splitter->handleWidth();
    if (pos >= width - handleWidth) pos = width;

    // Far left
    if (pos == 0) {
        ui->boardButtons->hide();
        ui->spacerLeft->changeSize(0, 0, QSizePolicy::Fixed);
        ui->spacerMidLeft->changeSize(0, 0, QSizePolicy::Fixed);
        ui->soundButtons->show();
        ui->spacerRight->changeSize(0, 0, QSizePolicy::Expanding);
        ui->spacerMidRight->changeSize(0, 0, QSizePolicy::Expanding);

        // Far right
    } else if (pos == ui->splitter->width()) {
        ui->boardButtons->show();
        ui->spacerLeft->changeSize(0, 0, QSizePolicy::Expanding);
        ui->spacerMidLeft->changeSize(0, 0, QSizePolicy::Expanding);
        ui->soundButtons->hide();
        ui->spacerRight->changeSize(0, 0, QSizePolicy::Fixed);
        ui->spacerMidRight->changeSize(0, 0, QSizePolicy::Fixed);

        // Center around the middle line
    } else {
        ui->boardButtons->show();
        ui->soundButtons->show();

        int middle = pos - (handleWidth / 2);
        int halfMuteButtonWidth = ui->muteButton->width() / 2;

        int muteButtonLeft = middle;
        int leftSpacerWidths = (muteButtonLeft - ui->boardButtons->width()) / 2;
        bool aboveZero = leftSpacerWidths > 0;

        ui->spacerLeft->changeSize(aboveZero ? leftSpacerWidths : 0, 0, aboveZero ? QSizePolicy::Maximum : QSizePolicy::Fixed);
        ui->spacerMidLeft->changeSize(aboveZero ? leftSpacerWidths : 0, 0, aboveZero ? QSizePolicy::Maximum : QSizePolicy::Fixed);

        int muteButtonRight = (leftSpacerWidths * 2) + ui->boardButtons->width() + halfMuteButtonWidth;
        int rightSpacerWidths = (width - muteButtonRight - ui->soundButtons->width()) / 2;
        aboveZero = rightSpacerWidths > 0;
        ui->spacerRight->changeSize(aboveZero ? rightSpacerWidths : 0, 0, aboveZero ? QSizePolicy::Maximum : QSizePolicy::Fixed);
        ui->spacerMidRight->changeSize(aboveZero ? rightSpacerWidths : 0, 0, aboveZero ? QSizePolicy::Maximum : QSizePolicy::Fixed);
    }

    // Update the appearance
    ui->buttonBar->layout()->update();
}

void Main::resizeEvent(QResizeEvent* event) {
    QMainWindow::resizeEvent(event);
    updateButtonBar();
}

void Main::restoreSizes() {
    // Restores the window geometry
    if (settings()->value(REMEMBER_WINDOW_SIZES, true).toBool()) {
        if (settings()->contains(WINDOW_MAIN_GEOMETRY)) restoreGeometry(settings()->value(WINDOW_MAIN_GEOMETRY).toByteArray());

        if (settings()->contains(WINDOW_MAIN_SPLITTER0) && settings()->contains(WINDOW_MAIN_SPLITTER1)) {
            QList<int> sizes;
            sizes.append(settings()->value(WINDOW_MAIN_SPLITTER0).toInt());
            sizes.append(settings()->value(WINDOW_MAIN_SPLITTER1).toInt());
            ui->splitter->setSizes(sizes);
        }
    } else {
        // Makes the "sounds" and "boards" sections equal width
        QList<int> sizes;
        sizes.append(ui->splitter->width() / 2);
        sizes.append(ui->splitter->width() / 2);
        ui->splitter->setSizes(sizes);
    }
    updateButtonBar();
}

void Main::showAudioEngineText(bool show) {
    if (show) ui->labelAudioEngine->show();
    else ui->labelAudioEngine->hide();
}

void Main::updateMuteButton() {
    // If it wants to show, and there's an ACTIVE input device
    if (_audio->activeInputs().size() > 0
            && _audio->inputObject()
            && ((_audio->inputObject()->isActiveOutput0() && _audio->getActiveDisplayOutput(0))
                || (_audio->inputObject()->isActiveOutput1() && _audio->getActiveDisplayOutput(1)))
            && settings()->value(SHOW_MUTE_BUTTON, true).toBool())
    {
        ui->muteButton->show();
        ui->muteButton->setIcon(QIcon(_audio->inputObject()->isMuted() ? ":/icons/res/mic_off.png" : ":/icons/res/mic_on.png"));
    }
    else ui->muteButton->hide();
}

void Main::on_muteButton_clicked()
{
    if (_audio->inputObject()) {
        _audio->inputObject()->setMute(!_audio->inputObject()->isMuted());
        updateMuteButton();
    }
}
