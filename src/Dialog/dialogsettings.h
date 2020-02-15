#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QComboBox>
#include "../Audio/audioobject.h"
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
    HostInfoContainer* displayHost;
    int deviceDisplayIndex;
};

class DialogSettings : public DialogTestAudio
{
    Q_OBJECT

public:
    explicit DialogSettings(Main *main);
    ~DialogSettings();

    void refreshDeviceSelection();
    void changeHost();
    void changeDevice();
    void audioEngineInit();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void host0Changed(int index);
    void device0Changed(int index);
    void host1Changed(int index);
    void device1Changed(int index);
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

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    HostInfoContainer *_displayHost0 = nullptr;
    HostInfoContainer *_displayHost1 = nullptr;
    void closeEvent(QCloseEvent *bar);
    void handleClose();
    void deviceChanged(QComboBox *selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer **displayHost);
    void setDeviceVolume(int value, int devDisplayIndex);
};

#endif // DIALOGSETTINGS_H
