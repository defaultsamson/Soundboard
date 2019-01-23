#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listitemboard.h"
#include "listitemsound.h"
#include "dialogboard.h"
#include "dialogsound.h"
#include "dialogsettings.h"

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    hasFile(false),
    ignoreBoardUpdate(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    this->clear();
    delete ui;
}

// ***************************** MENU BUTTONS *****************************

// New
void MainWindow::on_actionNew_triggered()
{
    this->clear();
    this->hasFile = false;
    this->fileName = "";
}

// Open
void MainWindow::on_actionOpen_triggered()
{
    this->load();
}

// Save
void MainWindow::on_actionSave_triggered()
{
    this->save();
}

// Save As
void MainWindow::on_actionSaveAs_triggered()
{
    this->save(true);
}

// Github Page
void MainWindow::on_actionGitHub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard", QUrl::TolerantMode));
}

// Update
void MainWindow::on_actionUpdate_triggered()
{
    // TODO check properly instead of just linking to the download page
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/releases", QUrl::TolerantMode));
}

// Wiki Page
void MainWindow::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/wiki", QUrl::TolerantMode));
}

// Exit program
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

// Settings (e.g. Sound set)
void MainWindow::on_actionSettings_triggered()
{
    DialogSettings w(this);
    w.exec();
}

// ***************************** END *****************************
// ***************************** BOARD BUTTONS *****************************

// Add board item
void MainWindow::on_buttonAddBoard_clicked()
{
    ListItemBoard *board = new ListItemBoard(this);
    ui->listBoards->addItem(board);
    this->editBoard(board, true);
}

// Remove board item
void MainWindow::on_buttonRemoveBoard_clicked()
{
    int row = ui->listBoards->currentRow();
    if (row < 0) return;
    ui->listBoards->takeItem(row);
}

// Edit board item
void MainWindow::on_buttonEditBoard_clicked()
{
    this->editBoard();
}

// Double clicked board, enter edit mode
void MainWindow::on_listBoards_itemActivated(QListWidgetItem *item)
{
    this->editBoard(static_cast<ListItemBoard*>(item));
}

// Display all the sounds from the selected board when it's selected
void MainWindow::on_listBoards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    ListItemBoard *board = static_cast<ListItemBoard*>(current);
    if (board) displayBoard(board);
}

// ***************************** END *****************************
// ***************************** SOUND BUTTONS *****************************

// Add sound item
void MainWindow::on_buttonAddSound_clicked()
{
    int row = ui->listBoards->currentRow();
    if (row < 0) return;
    ListItemBoard *board = static_cast<ListItemBoard*>(ui->listBoards->item(row));
    ListItemSound *sound = new ListItemSound(this, board);
    board->addSound(sound);
    this->displayBoard(board);
    this->editSound(sound, true);
}

// Remove sound item
void MainWindow::on_buttonRemoveSound_clicked()
{
    int row = ui->listSounds->currentRow();
    this->ui->listSounds->takeItem(row);
    ListItemBoard *board = static_cast<ListItemBoard*>(ui->listBoards->item(ui->listBoards->currentRow()));
    board->removeSound(row);
}

void MainWindow::on_buttonEditSound_clicked()
{
    this->editSound();
}

// ***************************** END *****************************

// Double clicked sound, enter edit mode
void MainWindow::on_listSounds_itemActivated(QListWidgetItem *item)
{
    this->editSound(static_cast<ListItemSound*>(item));
}

void MainWindow::displayBoard(ListItemBoard *board) {
    // TODO not do if settings don't want to update visuals

    if (board != ui->listBoards->currentItem()) {
        ignoreBoardUpdate = true;
        this->ui->listBoards->setCurrentItem(board);
        ignoreBoardUpdate = false;
    }
    board->populateList(this->ui->listSounds);
}

void MainWindow::clear() {
    for (int i = this->ui->listSounds->count() - 1; i >= 0; --i) this->ui->listSounds->takeItem(i);
    for (int i = this->ui->listBoards->count() - 1; i >= 0; --i) {
        delete this->ui->listBoards->takeItem(i);
    }
}

void MainWindow::load() {
    QString fn = QFileDialog::getOpenFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)"));

    QFile file(fn);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonDocument loadDoc(QJsonDocument::fromJson(file.readAll()));
    QJsonObject obj = loadDoc.object();
    QJsonArray arr = obj["boards"].toArray();

    this->hasFile = true;
    this->fileName = fn;

    this->clear();

    // Loads all the boards
    for (int i = 0; i < arr.size(); ++i) {
        ListItemBoard *board = new ListItemBoard(this);
        board->load(arr[i].toObject());
        this->ui->listBoards->addItem(board);
    }
}

void MainWindow::save() {
    this->save(false);
}

void MainWindow::save(bool saveAs) {
    if (!this->hasFile || saveAs) {
        // Let the user choose a save file
        this->fileName = QFileDialog::getSaveFileName(this, tr("Open Soundboard File"), QString(), tr("JSON Files (*.json)"));
    }

    QFile file(this->fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject json;

    // Creates an array of boards
    QJsonArray boards;
    for (int i = 0; i < this->ui->listBoards->count(); ++i) {
        QJsonObject b;
        static_cast<ListItemBoard*>(this->ui->listBoards->item(i))->save(b);
        boards.append(b);
    }

    json["boards"] = boards;

    // Turns the object into text and saves it
    QJsonDocument saveDoc(json);
    file.write(saveDoc.toJson());

    this->hasFile = true;
}

void MainWindow::editBoard(bool createNew) {
    this->editBoard(static_cast<ListItemBoard*>(ui->listBoards->item(ui->listBoards->currentRow())), createNew);
}

void MainWindow::editBoard(ListItemBoard *board, bool createNew) {
    if (!board) return;
    DialogBoard w(this, board, createNew);
    w.exec();
}

void MainWindow::editSound(bool createNew) {
    this->editSound(static_cast<ListItemSound*>(ui->listSounds->item(ui->listSounds->currentRow())), createNew);
}

void MainWindow::editSound(ListItemSound *sound, bool createNew) {
    if (!sound) return;
    DialogSound w(this, sound, createNew);
    w.exec();
}

void MainWindow::setCurrentBoard(ListItemBoard *board) {
    if (this->currentBoard) {
        this->currentBoard->unreg();
    }
    this->currentBoard = board;
    if (this->currentBoard) {
        this->currentBoard->reg();
        // TODO don't do visuals if the settins say not to
        this->displayBoard(this->currentBoard);
    }
}

void MainWindow::disableKeybinds() {
    for (int i = 0; i < this->ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(this->ui->listBoards->item(i))->unreg(true);
    }
}

void MainWindow::enableKeybinds() {
    for (int i = 0; i < this->ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(this->ui->listBoards->item(i))->reg(true);
    }
}

void MainWindow::removeBoard(ListItemBoard *board) {
    delete ui->listBoards->takeItem(ui->listBoards->row(board));
}

void MainWindow::removeSound(ListItemSound *sound) {
    ListItemBoard *board = sound->board();
    std::cout << "removing1: " << sound << std::endl;
    std::cout << "removing2: " << board << std::endl;
    board->removeSound(sound);
    std::cout << "removing3" << std::endl;
    displayBoard(board); // Note: board should always be currentBoard at this point
    std::cout << "removing4" << std::endl;
}
