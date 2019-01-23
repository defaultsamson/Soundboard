#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "listitemboard.h"
#include "listitemsound.h"
#include "dialogboard.h"
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
    hasFile(false)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    clear();
    delete ui;
}

// ******************* BGN MENU ACTIONS *******************

// New
void MainWindow::on_actionNew_triggered()
{
    clear();
    hasFile = false;
    fileName = "";
}

// Open
void MainWindow::on_actionOpen_triggered()
{
    load();
}

// Save
void MainWindow::on_actionSave_triggered()
{
    save();
}

// Save As
void MainWindow::on_actionSaveAs_triggered()
{
    save(true);
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

// Themes (e.g. dark theme, light theme, customizer??)
void MainWindow::on_actionTheme_triggered()
{

}

// Wiki Page
void MainWindow::on_actionWiki_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/wiki", QUrl::TolerantMode));
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

// ******************* END MENU ACTIONS *******************
// ******************* BGN LIST ACTIONS *******************

// Board item changed
void MainWindow::on_listBoards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    setCurrentBoard(static_cast<ListItemBoard*>(current));
}

// Board item double clicked, enter edit mode
void MainWindow::on_listBoards_itemActivated(QListWidgetItem *item)
{
    editBoard(static_cast<ListItemBoard*>(item));
}

// Sound item double clicked, enter edit mode
void MainWindow::on_listSounds_itemActivated(QListWidgetItem *item)
{
    // TODO
}

// ******************* END LIST ACTIONS *******************
// ******************* BGN BUTTON ACTIONS *******************

// Add board item
void MainWindow::on_buttonAddBoard_clicked()
{
    ListItemBoard *board = new ListItemBoard(this);
    ui->listBoards->addItem(board);
    editBoard(board, true);
}

// Remove board item
void MainWindow::on_buttonRemoveBoard_clicked()
{
    removeBoard(ui->listBoards->currentRow());
}

void MainWindow::on_buttonEditBoard_clicked()
{
    editBoard();
}

// Add sound item
void MainWindow::on_buttonAddSound_clicked()
{
    if (!currentBoard) return;
    ListItemSound *sound = new ListItemSound(this, currentBoard);
    currentBoard->addSound(sound);
    displayBoard(currentBoard);
    // TODO Edit sound
}

// Remove sound item
void MainWindow::on_buttonRemoveSound_clicked()
{
    removeSound(ui->listSounds->currentRow());
}

void MainWindow::on_buttonEditSound_clicked()
{
    // TODO
}

// ******************* END BUTTON ACTIONS *******************
// ******************* BGN BOARD FUNCTIONS *******************

void MainWindow::removeBoard(ListItemBoard *board) {
    removeBoard(ui->listBoards->row(board));
}

void MainWindow::removeBoard(int row) {
    if (row < 0 || row >= ui->listBoards->count()) return;
    delete ui->listBoards->takeItem(row);
}

void MainWindow::editBoard(bool createNew) {
    editBoard(static_cast<ListItemBoard*>(ui->listBoards->item(ui->listBoards->currentRow())), createNew);
}

void MainWindow::editBoard(ListItemBoard *board, bool createNew) {
    if (!board) return;
    DialogBoard w(this, board, createNew);
    w.exec();
}

void MainWindow::displayBoard(ListItemBoard *board) {
    // TODO not do if settings don't want to update visuals
    if (board != ui->listBoards->currentItem()) {
        ui->listBoards->setCurrentItem(board);
    }
    board->populateList(ui->listSounds);
}

void MainWindow::setCurrentBoard(ListItemBoard *board) {
    if (currentBoard) {
        currentBoard->unreg(false);
    }
    currentBoard = board;
    if (currentBoard) {
        currentBoard->reg(false);
        displayBoard(currentBoard);
    }
}

void MainWindow::playSound(ListItemSound *sounds) {
    ui->listSounds->setCurrentItem(sounds);
    // TODO play
}

void MainWindow::removeSound(ListItemSound *sound) {
    removeSound(ui->listSounds->row(sound));
}

void MainWindow::removeSound(int row) {
    if (row < 0 || row >= ui->listSounds->count()) return;
    delete ui->listSounds->takeItem(row);
    displayBoard(currentBoard);
}

// ******************* END BOARD FUNCTIONS *******************
// ******************* BGN FILE FUNCTIONS *******************

void MainWindow::clear() {
    for (int i = ui->listSounds->count() - 1; i >= 0; --i) ui->listSounds->takeItem(i);
    for (int i = ui->listBoards->count() - 1; i >= 0; --i) delete ui->listBoards->takeItem(i);
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

    hasFile = true;
    fileName = fn;

    clear();

    // Loads all the boards
    for (int i = 0; i < arr.size(); ++i) {
        ListItemBoard *board = new ListItemBoard(this);
        board->load(arr[i].toObject());
        ui->listBoards->addItem(board);
    }
    enableKeybinds();
}

void MainWindow::save(bool saveAs) {
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

void MainWindow::enableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(ui->listBoards->item(i))->reg(true);
    }
}

void MainWindow::disableKeybinds() {
    for (int i = 0; i < ui->listBoards->count(); ++i) {
        static_cast<ListItemBoard*>(ui->listBoards->item(i))->unreg(true);
    }
}

void MainWindow::trigger(ListItem *item) {
    item->trigger();
}
