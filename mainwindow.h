#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>

#include "listitem.h"
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
    void save(bool saveAs = false);
    void clear();
    void setCurrentBoard(ListItemBoard *board);
    void removeBoard(ListItemBoard *board);
    void removeBoard(int row);
    void removeSound(ListItemSound *sound);
    void removeSound(int row);
    void trigger(ListItem *item);
    void playSound(ListItemSound *sound);
    void setDarkTheme(bool set = true);

private slots:

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionSettings_triggered();
    void on_actionTheme_triggered();
    void on_actionWiki_triggered();
    void on_actionGitHub_triggered();
    void on_actionUpdate_triggered();

    void on_listBoards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_listBoards_itemActivated(QListWidgetItem *item);
    void on_listSounds_itemActivated(QListWidgetItem *item);

    void on_buttonAddBoard_clicked();
    void on_buttonRemoveBoard_clicked();
    void on_buttonEditBoard_clicked();
    void on_buttonAddSound_clicked();
    void on_buttonRemoveSound_clicked();
    void on_buttonEditSound_clicked();

    void on_actionTheme_toggled(bool checked);

public slots:
    void enableKeybinds();
    void disableKeybinds();

private:
    Ui::MainWindow *ui;
    bool hasFile;
    QString fileName;
    ListItemBoard *currentBoard;

    QPalette darkPalette;
    QPalette defaultPalette;

    void displayBoard(ListItemBoard *board);
    void editBoard(bool createNew = false);
    void editBoard(ListItemBoard *sound, bool createNew = false);
    void editSound(bool createNew = false);
    void editSound(ListItemSound *sound, bool createNew = false);
};

#endif // MAINWINDOW_H
