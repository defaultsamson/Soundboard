#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include "../Audio/audioobjectfile.h"
#include "../Audio/audioengine.h"
#include "dialogtestaudio.h"

#include <portaudio.h>

class Main;

namespace Ui {
class DialogSettings;
}

struct AudioDisplayContainer {
    QComboBox *drivers;
    QComboBox *devices;
    QPushButton *deleteButton;
    HostInfoContainer* displayHost;
    int deviceDisplayIndex;
    bool isInput;
};

class DialogSettings : public DialogTestAudio
{
    Q_OBJECT

public:
    explicit DialogSettings(Main* _main);
    ~DialogSettings();

    void refreshDeviceSelection();
    void changeHost();
    void changeDevice();
    void audioEngineInit();
    void updateScrollAreaPalette();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void onClose();

    void hostChangedOutput0(int index);
    void deviceChangedOutput0(int index);
    void hostChangedOutput1(int index);
    void deviceChangedOutput1(int index);
    void hostChangedInput0(int index);
    void deviceChangedInput0(int index);

    void on_checkBoxShowMuteButton_clicked();
    void on_checkBoxListDrivers_clicked();
    void on_buttonRefresh_clicked();
    void on_buttonPlay_clicked();
    void on_buttonPause_clicked();
    void on_buttonStop_clicked();
    void on_sliderVolumeOutput0_valueChanged(int value);
    void on_spinBoxVolumeOutput0_valueChanged(int arg1);
    void on_sliderVolumeOutput1_valueChanged(int value);
    void on_spinBoxVolumeOutput1_valueChanged(int arg1);
    void on_sliderVolumeInput0_valueChanged(int value);
    void on_spinBoxVolumeInput0_valueChanged(int arg1);
    void on_sliderVolumeAudioFile_valueChanged(int value);
    void on_spinBoxVolumeAudioFile_valueChanged(int arg1);
    void on_buttonDeleteOutput0_clicked();
    void on_buttonDeleteOutput1_clicked();
    void on_buttonDeleteInput0_clicked();
    void on_checkBoxInput0Output0_clicked();
    void on_checkBoxInput0Output1_clicked();
    void on_lineEditAudioFile_textEdited(const QString &arg1);
    void on_buttonMuteInput0_clicked();
    void on_buttonAudioFile_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_checkBoxDarkTheme_clicked();

private:
    Ui::DialogSettings* ui;
    Main* _main;
    AudioObjectFile audio;
    HostInfoContainer* _displayHost0 = nullptr;
    HostInfoContainer* _displayHost1 = nullptr;
    HostInfoContainer* _displayHostInput = nullptr;
    void outputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost);
    void inputChanged(QComboBox* selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer** displayHost);
    void outputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost);
    void inputRemoved(int deviceDisplayIndex, HostInfoContainer** displayHost);
    void setOutputDeviceVolume(int value, int devDisplayIndex);
    void updateFileName(QString fn);
    void updateMuteButton();
    bool _inputObjectInited = false;
    QPixmap modifyPixmap(QPixmap icon, bool dark, bool rotate);
    void updateGroupBoxes();
    void toggleOutput0();
    void toggleOutput1();
    void toggleInput0();
};

#endif // DIALOGSETTINGS_H
