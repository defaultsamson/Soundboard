#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include <QAudioDecoder>
#include "audiofilestream.h"
#include "audioobject.h"

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

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    bool pause = false;

    void initializeAudio(const QAudioDeviceInfo &deviceInfo);
};

#endif // DIALOGSETTINGS_H
