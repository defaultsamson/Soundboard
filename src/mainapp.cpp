#include "mainapp.h"
#include "ui_mainapp.h"

#include "Audio/audioengine.h"
#include "Widget/listitemboard.h"
#include "Widget/listitemsound.h"
#include "Dialog/dialogboard.h"
#include "Dialog/dialogsound.h"
#include "Dialog/dialogsettings.h"

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Main w;
    w.show();

    /*
    QString dir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    if (!dir.isEmpty()) {
        // TODO w.loadSettings(dir + "settings.json");
        // TODO w.saveSettings(dir + "settings.json");
        w.load(dir + "default.json");
    }*/

    return a.exec();
}

QString Main::DEFAULT_DIR = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/Soundboard/";
QString Main::SETTINGS_FILE = "settings.ini";
QString Main::DEFAULT_SOUNDBOARD = "default.json";
QString Main::DARK_THEME = "dark_theme";

Main::Main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainApp),
    _settings(new QSettings(DEFAULT_DIR + SETTINGS_FILE, QSettings::IniFormat)),
    defaultPalette(qApp->palette())
{
    ui->setupUi(this);

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

    setDarkTheme(settings()->value(DARK_THEME, false).toBool());
}

Main::~Main()
{
    clear();
    delete ui;
}

// ******************* BGN MENU ACTIONS *******************

// New
void Main::on_actionNew_triggered()
{
    clear();
    hasFile = false;
    fileName = "";
}

// Open
void Main::on_actionOpen_triggered()
{
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
void Main::on_listBoards_currentItemChanged(QListWidgetItem *current, QListWidgetItem * /* previous */)
{
    setCurrentBoard(static_cast<ListItemBoard*>(current));
}

// Board item double clicked, enter edit mode
void Main::on_listBoards_itemActivated(QListWidgetItem *item)
{
    editBoard(static_cast<ListItemBoard*>(item));
}

// Sound item double clicked, enter edit mode
void Main::on_listSounds_itemActivated(QListWidgetItem *item)
{
    editSound(static_cast<ListItemSound*>(item));
}

// ******************* END LIST ACTIONS *******************
// ******************* BGN BUTTON ACTIONS *******************

// Add board item
void Main::on_buttonAddBoard_clicked()
{
    ListItemBoard *board = new ListItemBoard(this);
    ui->listBoards->addItem(board);
    editBoard(board, true);
}

// Remove board item
void Main::on_buttonRemoveBoard_clicked()
{
    removeBoard(ui->listBoards->currentRow());
}

void Main::on_buttonEditBoard_clicked()
{
    editBoard();
}

// Add sound item
void Main::on_buttonAddSound_clicked()
{
    if (!currentBoard) return;
    ListItemSound *sound = new ListItemSound(this, currentBoard);
    currentBoard->addSound(sound);
    displayBoard(currentBoard);
    editSound(sound, true);
}

// Remove sound item
void Main::on_buttonRemoveSound_clicked()
{
    removeSound(ui->listSounds->currentRow());
}

void Main::on_buttonEditSound_clicked()
{
    editSound();
}

// ******************* END BUTTON ACTIONS *******************
// ******************* BGN BOARD FUNCTIONS *******************

void Main::removeBoard(ListItemBoard *board) {
    removeBoard(ui->listBoards->row(board));
}

void Main::removeBoard(int row) {
    if (row < 0 || row >= ui->listBoards->count()) return;
    delete ui->listBoards->takeItem(row);
}

void Main::editBoard(bool createNew) {
    editBoard(static_cast<ListItemBoard*>(ui->listBoards->item(ui->listBoards->currentRow())), createNew);
}

void Main::editBoard(ListItemBoard *board, bool createNew) {
    if (!board) return;
    DialogBoard w(this, board, createNew);
    w.exec();
}

void Main::displayBoard(ListItemBoard *board) {
    // TODO not do if settings don't want to update visuals
    if (board != ui->listBoards->currentItem()) {
        ui->listBoards->setCurrentItem(board);
    }
    board->populateList(ui->listSounds);
}

void Main::setCurrentBoard(ListItemBoard *board) {
    if (currentBoard == board) return;
    if (currentBoard) {
        currentBoard->unreg(false);
    }
    currentBoard = board;
    if (currentBoard) {
        currentBoard->reg(false, true);
        displayBoard(currentBoard);
    }
}

void Main::setCurrentSound(ListItemSound *sound) {
    // TODO don't do display if the settings say not to
    ui->listSounds->setCurrentItem(sound);
    // TODO play audio
}

void Main::removeSound(ListItemSound *sound) {
    removeSound(ui->listSounds->row(sound));
}

void Main::removeSound(int row) {
    if (row < 0 || row >= ui->listSounds->count()) return;
    delete ui->listSounds->takeItem(row);
    displayBoard(currentBoard);
}

void Main::editSound(bool createNew) {
    editSound(static_cast<ListItemSound*>(ui->listBoards->item(ui->listBoards->currentRow())), createNew);
}

void Main::editSound(ListItemSound *sound, bool createNew) {
    if (!sound) return;
    DialogSound w(this, sound, createNew);
    w.exec();
}

void Main::clear() {
    for (int i = ui->listSounds->count() - 1; i >= 0; --i) ui->listSounds->takeItem(i);
    for (int i = ui->listBoards->count() - 1; i >= 0; --i) delete ui->listBoards->takeItem(i);
}

// ******************* END BOARD FUNCTIONS *******************
// ******************* BGN FILE FUNCTIONS *******************

void Main::load() {
    load(QFileDialog::getOpenFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)")));
}

void Main::load(const QString fn) {
    QFile file(fn);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    QJsonObject obj = loadDoc.object();
    QJsonArray arr = obj["boards"].toArray();

    hasFile = true;
    fileName = fn;

    clear();

    // Loads all the boards
    for (int i = 0; i < arr.size(); ++i) {
        ListItemBoard *board = new ListItemBoard(this);
        board->load(arr[i].toObject());
        ui->listBoards->addItem(board);
        // Set the first board as the active board
        if (i == 0) {
            setCurrentBoard(board);
        }
    }
    enableKeybinds();
}

void Main::save(bool saveAs) {
    if (!hasFile || saveAs) {
        // Let the user choose a save file
        fileName = QFileDialog::getSaveFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)"));
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject json;

    // Creates an array of boards
    QJsonArray boards;
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        QJsonObject b;
        static_cast<ListItemBoard*>(ui->listBoards->item(i))->save(b);
        boards.append(b);
    }

    json["boards"] = boards;

    // Turns the object into text and saves it
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());

    hasFile = true;
}

// ******************* END FILE FUNCTIONS *******************

void Main::enableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(ui->listBoards->item(i))->reg(true, false);
    }
    if (currentBoard) currentBoard->reg(false, true);
}

void Main::disableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(ui->listBoards->item(i))->unreg(true);
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
