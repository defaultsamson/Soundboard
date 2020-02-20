#ifndef MAINAPP_H
#define MAINAPP_H

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
#include "Hotkey/hotkeytrigger.h"

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
    static QString DEFAULT_TEST_FILE;

    static QString DARK_THEME;
    static bool DARK_THEME_DEFAULT;
    static QString ACTIVE_BOARD;
    static QString ACTIVE_BOARD_DEFAULT;
    static QString HAS_ACTIVE_BOARD;
    static bool HAS_ACTIVE_BOARD_DEFAULT;
    static QString SETTINGS_TAB;

    static QString HK_ENABLE_KEYBINDS_HAS;
    static QString HK_ENABLE_KEYBINDS_KEY;
    static QString HK_DISABLE_KEYBINDS_HAS;
    static QString HK_DISABLE_KEYBINDS_KEY;
    static QString HK_STOP_SOUNDS_HAS;
    static QString HK_STOP_SOUNDS_KEY;
    static QString HK_PAUSE_SOUNDS_HAS;
    static QString HK_PAUSE_SOUNDS_KEY;
    static QString HK_RESUME_SOUNDS_HAS;
    static QString HK_RESUME_SOUNDS_KEY;
    static QString HK_MUTE_INPUT_HAS;
    static QString HK_MUTE_INPUT_KEY;
    static QString HK_UNMUTE_INPUT_HAS;
    static QString HK_UNMUTE_INPUT_KEY;
    static QString HK_TOGGLE_MUTE_INPUT_HAS;
    static QString HK_TOGGLE_MUTE_INPUT_KEY;

    static QString REMEMBER_WINDOW_SIZES;
    static QString WINDOW_MAIN_GEOMETRY;
    static QString WINDOW_MAIN_SPLITTER0;
    static QString WINDOW_MAIN_SPLITTER1;
    static QString WINDOW_SETTINGS_WIDTH;
    static QString WINDOW_SETTINGS_HEIGHT;
    static QString WINDOW_BOARD_WIDTH;
    static QString WINDOW_BOARD_HEIGHT;
    static QString WINDOW_SOUND_WIDTH;
    static QString WINDOW_SOUND_HEIGHT;
    static QString SHOW_SETTINGS_OUTPUT0;
    static QString SHOW_SETTINGS_OUTPUT1;
    static QString SHOW_SETTINGS_INPUT0;

    static QString EXPLICIT_NO_OUTPUT_DEVICES;
    static QString EXPLICIT_NO_INPUT_DEVICES;
    static QString OUTPUT_INDEX0;
    static QString OUTPUT_INDEX1;
    static QString INPUT_INDEX0;
    static QString OUTPUT_VOLUME0;
    static QString OUTPUT_VOLUME1;
    static QString INPUT_VOLUME0;
    static QString TEST_VOLUME;
    static QString INPUT_OUT0;
    static QString INPUT_OUT1;
    static QString INPUT_MUTED;
    static QString SHOW_MUTE_BUTTON;
    static QString SHOW_DRIVERS;
    static QString TEST_FILE;

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

    HotkeyTrigger* hkEnableKeybinds;
    HotkeyTrigger* hkDisableKeybinds;
    HotkeyTrigger* hkStopSounds;
    HotkeyTrigger* hkPauseSounds;
    HotkeyTrigger* hkResumeSounds;
    HotkeyTrigger* hkMuteInput;
    HotkeyTrigger* hkUnmuteInput;
    HotkeyTrigger* hkToggleMuteInput;

    void restoreSizes();
    void updateButtonBar();
    void showAudioEngineText(bool show);

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

    void on_splitter_splitterMoved(int pos, int index);

    void on_muteButton_clicked();

public slots:
    void enableKeybinds();
    void disableKeybinds();
    void updateMuteButton();

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
    void updateButtonBar(int pos);
    void resizeEvent(QResizeEvent* event);
    QMessageBox::StandardButton unsavedChangedDialogue();
    void closeEvent(QCloseEvent* bar);
};

#endif // MAINAPP_H
