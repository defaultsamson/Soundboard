#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include "board.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Board *board1 = new Board("First");
    Board *board2 = new Board("Second");
    Board *board3 = new Board("Third");

    ui->listBoards->addItem(board1);
    ui->listBoards->addItem(board2);
    ui->listBoards->addItem(board3);

    board1->addSound(new Sound("Nut 1"));
    board1->addSound(new Sound("Nut 2"));
    board1->addSound(new Sound("Nut 3"));
    board1->addSound(new Sound("Nut 4"));
    board1->addSound(new Sound("Nut 5"));

    board3->addSound(new Sound("Succ 1"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Save
void MainWindow::on_actionSave_triggered()
{

}

// Save As
void MainWindow::on_actionSaveAs_triggered()
{

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

}

// BOARDS
// Double clicked, enter edit mode
void MainWindow::on_listBoards_itemActivated(QListWidgetItem *item)
{
    setDebug("[BOARDS] Item Activated: " + std::to_string(ui->listBoards->currentRow()));
}

// SOUNDS
// Double clicked, enter edit mode
void MainWindow::on_listSounds_itemActivated(QListWidgetItem *item)
{
    setDebug("[SOUNDS] Item Activated: " + std::to_string(ui->listBoards->currentRow()));
}

void MainWindow::setDebug(std::string s) {
    ui->label_3->setText(QString::fromStdString(s));
}

// BOARDS
// Row change, load up the sounds in this row
void MainWindow::on_listBoards_currentRowChanged(int currentRow)
{
    setDebug("[BOARDS] Row Change: " + std::to_string(currentRow));
    this->displayBoard(currentRow);
}

// Add board item
void MainWindow::on_buttonAddBoard_clicked()
{
    ui->listBoards->addItem(new Board("New Board"));
    // TODO enter edit mode
}

// Remove board item
void MainWindow::on_buttonRemoveBoard_clicked()
{
    ui->listBoards->takeItem(ui->listBoards->currentRow());
}

// Add sound item
void MainWindow::on_buttonAddSound_clicked()
{
    int row = ui->listBoards->currentRow();
    Sound *sound = new Sound("New Sound");
    Board *board = static_cast<Board*>(ui->listBoards->item(row));
    board->addSound(sound);
    this->displayBoard(board);
    // TODO Edit sound
}

// Remove sound item
void MainWindow::on_buttonRemoveSound_clicked()
{
    int row = ui->listSounds->currentRow();
    this->ui->listSounds->takeItem(row);
    Board *board = static_cast<Board*>(ui->listBoards->item(ui->listBoards->currentRow()));
    board->removeSound(row);
}

void MainWindow::displayBoard(int row) {
    this->displayBoard(static_cast<Board*>(ui->listBoards->item(row)));
}

void MainWindow::displayBoard(Board *board) {
    board->populateList(this->ui->listSounds);
}
