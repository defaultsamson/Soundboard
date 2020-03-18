#ifndef MAINAPP_H
#define MAINAPP_H

#include "Audio/audioengine.h"
#include "Dialog/dialogtestaudio.h"
#include "Widget/listitemboard.h"
#include "Widget/listitemsound.h"
#include "Hotkey/hotkeytrigger.h"

#include <QMainWindow>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>
#include <QModelIndex>
#include <QMessageBox>

namespace Ui {
class MainApp;
}

class Main : public QMainWindow
{
    Q_OBJECT

public:
    Main(QWidget* parent = nullptr);
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
    AudioEngine& audio();

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
    AudioEngine _audio;
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
