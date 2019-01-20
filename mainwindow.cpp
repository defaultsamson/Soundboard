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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_triggered()
{
    // Save
}

void MainWindow::on_actionSave_As_triggered()
{
    // Save As
}

void MainWindow::on_actionGitHub_Page_triggered()
{
    // Github Page
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard", QUrl::TolerantMode));
}

void MainWindow::on_actionCheck_For_Updates_triggered()
{
    // TODO check properly instead of just linking to the download page
    // Update
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/releases", QUrl::TolerantMode));
}

void MainWindow::on_actionWiki_triggered()
{
    // Wiki Page
    QDesktopServices::openUrl(QUrl("https://github.com/qwertysam/Soundboard/wiki", QUrl::TolerantMode));
}

void MainWindow::on_actionExit_triggered()
{
    // Exit program
    QApplication::quit();
}

void MainWindow::on_actionSettings_triggered()
{
    // Settings (e.g. Sound set)
}

void MainWindow::on_listWidget_2_itemActivated(QListWidgetItem *item)
{
    // BOARDS
    // Double clicked, enter edit mode
    setDebug("[BOARDS] Item Activated: " + std::to_string(ui->listWidget_2->currentRow()));
}

void MainWindow::on_listWidget_itemActivated(QListWidgetItem *item)
{
    // SOUNDS
    // Double clicked, enter edit mode
    setDebug("[SOUNDS] Item Activated: " + std::to_string(ui->listWidget_2->currentRow()));
}

void MainWindow::setDebug(std::string s) {
    ui->label_3->setText(QString::fromStdString(s));
}

void MainWindow::on_listWidget_2_currentRowChanged(int currentRow)
{
    // BOARDS
    // Row change, load up the sounds in this row
    setDebug("[BOARDS] Row Change: " + std::to_string(currentRow));
}

void MainWindow::on_pushButton_3_clicked()
{
    // Add board item
    ui->listWidget_2->addItem(new Board("noice"));
}

void MainWindow::on_pushButton_2_clicked()
{
    // Remove board item
    ui->listWidget_2->takeItem(ui->listWidget_2->currentRow());
}
