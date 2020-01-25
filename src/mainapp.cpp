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
#include <QModelIndex>
#include <QObject>
#include <QAbstractItemModel>

#include <iostream>

#include <portaudio.h>

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
    _audio(new AudioEngine()),
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

    setDarkTheme(settings()->value(DARK_THEME, false).toBool());

    std::cout << "Beginning" << std::endl;

    const PaDeviceInfo *device;

    Pa_Initialize();

    for(int i = 0; i < Pa_GetDeviceCount(); ++i ) {
        device = Pa_GetDeviceInfo(i);
        std::cout << "Device: " << Pa_GetDeviceInfo(i)->hostApi << std::endl;
        if( Pa_GetDeviceInfo(i)->hostApi == Pa_HostApiTypeIdToHostApiIndex(paALSA) )
        {
            std::cout << "------------------------------------------" << std::endl;
            std::cout << "ALSA Device: " << device->name << std::endl;
            if(device->maxOutputChannels == 0) // isInput
                std::cout << "  (Input) " << device->maxInputChannels << " Channels" << std::endl;
            else
                std::cout << "  (Output) " << device->maxOutputChannels << " Channels" << std::endl;
        } else if( Pa_GetDeviceInfo(i)->hostApi == Pa_HostApiTypeIdToHostApiIndex(paJACK) )
        {
            std::cout << "------------------------------------------" << std::endl;
            std::cout << "JACK Device: " << device->name << std::endl;
            if(device->maxOutputChannels == 0) // isInput
                std::cout << "  (Input) " << device->maxInputChannels << " Channels" << std::endl;
            else
                std::cout << "  (Output) " << device->maxOutputChannels << " Channels" << std::endl;
        }
    }
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
        setCurrentBoard(static_cast<ListItemBoard*>(ui->listBoards->item(row)));
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

void Main::onSoundMoved(const QModelIndex &/*parent*/, int start, int /*end*/, const QModelIndex &/*destination*/, int row) {
    // We can ignore "end" since we know that there will always only be one item being moved
    if (row > start) row -= 1; // This is because the row variable refers to the index of the space inbetween each item index, this converts it to the item index itself
    // std::cout << "Moved from " << start << " to " << row << std::endl;
    if (start >= 0 && row >= 0) currentBoard->moveSound(static_cast<size_t>(start), static_cast<size_t>(row));
}

void Main::on_listSounds_itemClicked(QListWidgetItem */*item*/) {
    ui->buttonEditSound->setEnabled(true);
    ui->buttonRemoveSound->setEnabled(true);
}

void Main::on_listBoards_itemClicked(QListWidgetItem */*item*/) {
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
    ListItemBoard *board = new ListItemBoard(this);
    ui->listBoards->addItem(board);
    setCurrentBoard(board);
    editBoard(board, true);
}

void Main::addSound() {
    if (!currentBoard) return;
    ListItemSound *sound = new ListItemSound(this, currentBoard);
    currentBoard->addSound(sound);
    displayBoard(currentBoard);
    setCurrentSound(sound);
    ui->buttonEditSound->setEnabled(true);
    ui->buttonRemoveSound->setEnabled(true);
    editSound(sound, true);
}

void Main::removeBoard(ListItemBoard *board) {
    removeBoard(ui->listBoards->row(board));
}

void Main::removeBoard(int row) {
    if (row < 0 || row >= ui->listBoards->count()) return;
    delete ui->listBoards->takeItem(row);
}

void Main::editBoard(bool createNew) {
    editBoard(currentBoard, createNew);
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
    ui->buttonAddSound->setEnabled(true);
    ui->buttonEditSound->setEnabled(ui->listSounds->currentRow() >= 0); // highlight if a sound is selected
    ui->buttonRemoveSound->setEnabled(ui->listSounds->currentRow() >= 0); // highlight if a sound is selected
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

        ui->buttonEditBoard->setEnabled(true);
        ui->buttonRemoveBoard->setEnabled(true);
    } else {

        ui->buttonEditBoard->setEnabled(false);
        ui->buttonRemoveBoard->setEnabled(false);

        ui->buttonAddSound->setEnabled(false);
        ui->buttonEditSound->setEnabled(false);
        ui->buttonRemoveSound->setEnabled(false);
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
    editSound(currentSound(), createNew);
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

ListItemSound* Main::currentSound() {
    return static_cast<ListItemSound*>(ui->listSounds->item(ui->listSounds->currentRow()));
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
