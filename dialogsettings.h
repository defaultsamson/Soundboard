#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include <QAudioDecoder>
#include "audiofilestream.h"
#include "audioobject.h"

class MainWindow;

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(MainWindow *main = nullptr);
    ~DialogSettings();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonOutput_clicked();
    void deviceChanged(int index);

private:
    Ui::DialogSettings *ui;
    MainWindow *main;
    void initializeAudio(const QAudioDeviceInfo &deviceInfo);
    AudioObject audio;
    bool pause = false;
};

#endif // DIALOGSETTINGS_H
