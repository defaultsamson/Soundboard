#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>
#include <QSettings>
#include <QModelIndex>

#include "Audio/audioengine.h"
#include "Widget/listitem.h"
#include "Widget/listitemboard.h"
#include "Widget/listitemsound.h"

namespace Ui {
class MainApp;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    static QString DEFAULT_DIR;
    static QString SETTINGS_FILE;
    static QString DEFAULT_SOUNDBOARD;
    static QString DARK_THEME;
    explicit Main(QWidget *parent = nullptr);
    ~Main();
    void load();
    void load(const QString fn);
    void save(bool saveAs = false);
    void clear();
    void setCurrentBoard(ListItemBoard *board);
    void removeBoard(ListItemBoard *board);
    void removeBoard(int row);
    void setCurrentSound(ListItemSound *sound);
    void removeSound(ListItemSound *sound);
    void removeSound(int row);
    void setDarkTheme(bool set = true);
    QSettings *settings();
    AudioEngine *audio();
private slots:

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void on_actionExit_triggered();
    void on_actionSettings_triggered();
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

    void onSoundMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void contextMenuBoard(const QPoint &pos);
    void contextMenuSound(const QPoint &pos);

    void on_listSounds_itemClicked(QListWidgetItem *item);
    void on_listBoards_itemClicked(QListWidgetItem *item);

public slots:
    void enableKeybinds();
    void disableKeybinds();

private:
    Ui::MainApp *ui;
    bool hasFile = false;
    QString fileName;
    ListItemBoard *currentBoard = nullptr;
    AudioEngine *_audio;
    QSettings *_settings;

    QPalette darkPalette;
    QPalette defaultPalette;

    void displayBoard(ListItemBoard *board);
    void editBoard(bool createNew = false);
    void editBoard(ListItemBoard *sound, bool createNew = false);
    void editSound(bool createNew = false);
    void editSound(ListItemSound *sound, bool createNew = false);
    void addBoard();
    void addSound();
    ListItemSound *currentSound();
};

#endif // MAINWINDOW_H
