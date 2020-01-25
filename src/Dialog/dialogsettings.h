#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAudioOutput>
#include <QFile>
#include <QAudioDecoder>
#include "../Audio/audiofilestream.h"
#include "../Audio/audioobject.h"

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
    void deviceChanged(int index);
    void on_checkBoxDarkTheme_stateChanged(int arg1);

    void on_pushButtonRefresh_clicked();

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    bool pause = false;

    void initializeAudio(DeviceInfoContainer);
    void refreshDeviceSelection();
};

#endif // DIALOGSETTINGS_H
