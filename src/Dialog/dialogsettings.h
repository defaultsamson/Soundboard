#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAudioOutput>
#include <QFile>
#include <QAudioDecoder>
#include "../Audio/audioobject.h"
#include "../Audio/audioengine.h"

#include <portaudio.h>

class Main;

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(Main *main = nullptr);
    ~DialogSettings();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonOutput_clicked();
    void hostChanged(int index);
    void deviceChanged(int index);
    void on_checkBoxDarkTheme_stateChanged(int arg1);
    void on_pushButtonRefresh_clicked();

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    bool pause = false;
    bool _hasDisplayHost = false;
    HostInfoContainer _displayHost;

    void initializeAudio(DeviceInfoContainer);
    void refreshDeviceSelection();
};

#endif // DIALOGSETTINGS_H
