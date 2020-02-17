#include "mainapp.h"
#include "ui_mainapp.h"

#include "Audio/audioengine.h"
#include "Widget/listitemboard.h"
#include "Widget/listitemsound.h"
#include "Dialog/dialogboard.h"
#include "Dialog/dialogsound.h"
#include "Dialog/dialogsettings.h"
#include "Dialog/dialogtestaudio.h"

#include <QApplication>
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

class MyThread : public QThread
{
public:
    MyThread(Main &main) : main(main) {}
protected:
    void run();
private:
    Main &main;
};        

void MyThread::run()
{
    // Set up testing audio
    QFile testFile(Main::TEST_AUDIO);
    if (!testFile.exists()) {
        QFile::copy(":/audio/res/test.ogg", Main::TEST_AUDIO);
    }

    main.audio()->init();
    if (main.getAudioTestDialog()) main.getAudioTestDialog()->audioEngineInit();
}

void Main::setAudioTestDialog(DialogTestAudio *s) {
    _audioTestDialog = s;
}
DialogTestAudio *Main::getAudioTestDialog() {
    return _audioTestDialog;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main w;
    w.show();

    // Initializes audio engine on separate thread so that the UI starts super fast
    MyThread t(w);
    t.start();

    return a.exec();
}

QString Main::DEFAULT_DIR = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Soundboard/";
QString Main::SETTINGS_FILE = "settings.ini";
QString Main::DEFAULT_SOUNDBOARD = "default.json";
QString Main::TEST_AUDIO = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/test.ogg";

QString Main::DARK_THEME = "dark_theme";
bool Main::DARK_THEME_DEFAULT = false;
QString Main::ACTIVE_BOARD = "active_board";
QString Main::ACTIVE_BOARD_DEFAULT = "";
QString Main::HAS_ACTIVE_BOARD = "has_active_board";
bool Main::HAS_ACTIVE_BOARD_DEFAULT = false;
QString Main::SETTINGS_TAB = "settings_tab";

QString Main::EXPLICIT_NO_OUTPUT_DEVICES = "explicit_no_outputs";
QString Main::EXPLICIT_NO_INPUT_DEVICES = "explicit_no_inputs";
QString Main::OUTPUT_INDEX0 = "output_index0";
QString Main::OUTPUT_INDEX1 = "output_index1";
QString Main::INPUT_INDEX0 = "input_index0";
QString Main::OUTPUT_VOLUME0 = "output_volume0";
QString Main::OUTPUT_VOLUME1 = "output_volume1";
QString Main::INPUT_VOLUME0 = "input_volume0";
QString Main::TEST_VOLUME = "test_volume";

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
}

Main::~Main()
{
    clear();
    delete ui;
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
        }
    } else {
        event->accept();
    }
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
