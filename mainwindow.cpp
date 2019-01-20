#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    selectedBoard(0),
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
