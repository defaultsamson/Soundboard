#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>
#include <QSettings>
#include <QModelIndex>
#include <QMessageBox>

#include "Audio/audioengine.h"
#include "Dialog/dialogsettings.h"
#include "Dialog/dialogtestaudio.h"
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
    static QString TEST_AUDIO;

    static QString DARK_THEME;
    static bool DARK_THEME_DEFAULT;
    static QString ACTIVE_BOARD;
    static QString ACTIVE_BOARD_DEFAULT;
    static QString HAS_ACTIVE_BOARD;
    static bool HAS_ACTIVE_BOARD_DEFAULT;
    static QString SETTINGS_TAB;

    static QString EXPLICIT_NO_OUTPUT_DEVICES;
    static QString EXPLICIT_NO_INPUT_DEVICES;
    static QString OUTPUT_INDEX0;
    static QString OUTPUT_INDEX1;
    static QString INPUT_INDEX0;
    static QString OUTPUT_VOLUME0;
    static QString OUTPUT_VOLUME1;
    static QString INPUT_VOLUME0;
    static QString TEST_VOLUME;

    explicit Main(QWidget* parent = nullptr);
    ~Main();
    void load();
    void load(const QString fn);
    void save(bool saveAs = false);
    void clear();
    void setCurrentBoard(ListItemBoard* board);
    void setCurrentBoard(int row);
    void removeBoard(ListItemBoard* board, bool wasNew = false);
    void removeBoard(int row, bool wasNew = false);
    ListItemBoard* getBoard(int row);
    void setCurrentSound(ListItemSound* sound);
    void setCurrentSound(int row);
    void removeSound(ListItemSound* sound, bool wasNew = false);
    void removeSound(int row, bool wasNew = false);
    ListItemSound* getSound(int row);
    void setDarkTheme(bool set = true);
    QSettings* settings();
    AudioEngine* audio();

    // This is only used while initializing the AudioEngine, makes the audio player work better
    void setAudioTestDialog(DialogTestAudio*);
    DialogTestAudio* getAudioTestDialog();

    void setChanged(bool changed = true);

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

    void on_listBoards_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
    void on_listBoards_itemActivated(QListWidgetItem* item);
    void on_listSounds_itemActivated(QListWidgetItem* item);

    void on_buttonAddBoard_clicked();
    void on_buttonRemoveBoard_clicked();
    void on_buttonEditBoard_clicked();
    void on_buttonAddSound_clicked();
    void on_buttonRemoveSound_clicked();
    void on_buttonEditSound_clicked();

    void onSoundMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void contextMenuBoard(const QPoint &pos);
    void contextMenuSound(const QPoint &pos);

    void on_listSounds_itemClicked(QListWidgetItem* item);
    void on_listBoards_itemClicked(QListWidgetItem* item);

    void on_buttonPlay_clicked();
    void on_buttonPause_clicked();
    void on_buttonStop_clicked();

    void rowChanged(int row);

public slots:
    void enableKeybinds();
    void disableKeybinds();

private:
    Ui::MainApp* ui;
    AudioEngine* _audio;
    QSettings* _settings;
    bool hasFile = false;
    QString fileName;
    ListItemBoard* currentBoard = nullptr;
    bool _changed = false;

    DialogTestAudio* _audioTestDialog = nullptr;

    QPalette darkPalette;
    QPalette defaultPalette;

    void displayBoard(ListItemBoard* board);
    void editBoard(bool createNew = false);
    void editBoard(ListItemBoard* sound, bool createNew = false);
    void editSound(bool createNew = false);
    void editSound(ListItemSound* sound, bool createNew = false);
    void addBoard();
    void addSound();
    ListItemSound* currentSound();
    void updateTitle();
    QMessageBox::StandardButton unsavedChangedDialogue();
    void closeEvent(QCloseEvent* bar);
};

#endif // MAINWINDOW_H
