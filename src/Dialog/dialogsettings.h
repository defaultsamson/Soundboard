#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QFile>
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

    void refreshDeviceSelection();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonOutput_clicked();
    void hostChanged(int index);
    void deviceChanged(int index);
    void on_checkBoxDarkTheme_stateChanged(int arg1);
    void on_pushButtonRefresh_clicked();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    bool _hasDisplayHost = false;
    HostInfoContainer _displayHost;
    void closeEvent(QCloseEvent *bar);
    void handleClose();
};

#endif // DIALOGSETTINGS_H
