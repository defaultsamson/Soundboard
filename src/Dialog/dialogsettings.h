#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QComboBox>
#include "../Audio/audioobject.h"
#include "../Audio/audioengine.h"

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

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(Main *main = nullptr);
    ~DialogSettings();

    void refreshDeviceSelection();
    void changeHost();
    void changeDevice();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonOutput_clicked();
    void host1Changed(int index);
    void device1Changed(int index);
    void host2Changed(int index);
    void device2Changed(int index);
    void on_checkBoxDarkTheme_stateChanged(int arg1);
    void on_pushButtonRefresh_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::DialogSettings *ui;
    Main *main;
    AudioObject audio;
    HostInfoContainer *_displayHost1 = nullptr;
    HostInfoContainer *_displayHost2 = nullptr;
    void closeEvent(QCloseEvent *bar);
    void handleClose();
    void deviceChanged(QComboBox *selector, int selectorIndex, int deviceDisplayIndex, HostInfoContainer *displayHost);
};

#endif // DIALOGSETTINGS_H
