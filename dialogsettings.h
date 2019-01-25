#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QFile>
#include <QAudioDecoder>
#include "audiofilestream.h"

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

private:
    Ui::DialogSettings *ui;
    MainWindow *main;
    void deviceChanged(int index);
    void initializeAudio(const QAudioDeviceInfo &deviceInfo);
    QAudioOutput *audio;
    AudioFileStream *stream;
    QAudioDeviceInfo deviceInfo;
};

#endif // DIALOGSETTINGS_H
