#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>

#include "listitemboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void load();
    void save();
    void save(bool saveAs);
    void clear();
    void setCurrentBoard(ListItemBoard *board);

private slots:

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_actionGitHub_triggered();

    void on_actionUpdate_triggered();

    void on_actionWiki_triggered();

    void on_actionExit_triggered();

    void on_actionSettings_triggered();

    void on_listBoards_itemActivated(QListWidgetItem *item);

    void on_listSounds_itemActivated(QListWidgetItem *item);

    void on_listBoards_currentRowChanged(int currentRow);

    void on_buttonAddBoard_clicked();

    void on_buttonRemoveBoard_clicked();

    void on_buttonAddSound_clicked();

    void on_buttonRemoveSound_clicked();

    void on_actionOpen_triggered();

    void on_buttonEditBoard_clicked();

    void on_actionNew_triggered();

private:
    Ui::MainWindow *ui;
    void setDebug(std::string st);
    void displayBoard(int row);
    void displayBoard(ListItemBoard *board);
    void boardEdit();
    void boardEdit(ListItemBoard *board);
    bool hasFile;
    QString fileName;
    ListItemBoard *currentBoard;
};

#endif // MAINWINDOW_H
