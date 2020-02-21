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

    void host0Changed(int index);
    void device0Changed(int index);
    void host1Changed(int index);
    void device1Changed(int index);
    void hostInputChanged(int index);
    void deviceInputChanged(int index);

    void on_checkBoxDarkTheme_stateChanged(int arg1);
    void on_pushButtonRefresh_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pushButtonPlay_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();

    void on_sliderDevice0_valueChanged(int value);
    void on_spinBoxDevice0_valueChanged(int arg1);
    void on_sliderDevice1_valueChanged(int value);
    void on_spinBoxDevice1_valueChanged(int arg1);
    void on_sliderInput_valueChanged(int value);
    void on_spinBoxInput_valueChanged(int arg1);

    void on_sliderTest_valueChanged(int value);
    void on_spinBoxTest_valueChanged(int arg1);

    void on_deleteButtonDevice0_clicked();
    void on_deleteButtonDevice1_clicked();
    void on_deleteButtonDeviceInput_clicked();

    void on_checkBoxInput0_clicked();
    void on_checkBoxInput1_clicked();

    void on_pushButtonTestFile_clicked();
    void on_lineEditTestFile_textEdited(const QString &arg1);

    void on_muteButton_clicked();

    void on_checkBoxShowMuteButton_clicked();

    void on_checkBoxListDrivers_clicked();

private:
    Ui::DialogSettings* ui;
    Main* _main;
    AudioObjectFile audio;
    HostInfoContainer* _displayHost0 = nullptr;
    HostInfoContainer* _displayHost1 = nullptr;
    HostInfoContainer* _displayHostInput = nullptr;
    void closeEvent(QCloseEvent* bar);
    void handleClose();
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
