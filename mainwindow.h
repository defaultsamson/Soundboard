#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>

#include "listitemboard.h"
#include "listitemsound.h"

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
    void removeBoard(ListItemBoard *board);
    void removeSound(ListItemSound *sound);
    ListItemBoard *currentBoard;

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

    void on_buttonAddBoard_clicked();

    void on_buttonRemoveBoard_clicked();

    void on_buttonAddSound_clicked();

    void on_buttonRemoveSound_clicked();

    void on_actionOpen_triggered();

    void on_buttonEditBoard_clicked();

    void on_actionNew_triggered();

    void on_buttonEditSound_clicked();

    void on_listBoards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

public slots:
    void disableKeybinds();
    void enableKeybinds();

private:
    void displayBoard(ListItemBoard *board);
    void editBoard(bool createNew = false);
    void editBoard(ListItemBoard *board, bool createNew = false);
    void editSound(bool createNew = false);
    void editSound(ListItemSound *sound, bool createNew = false);
    bool hasFile;
    QString fileName;
    bool ignoreBoardUpdate;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
