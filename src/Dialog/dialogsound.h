#ifndef DIALOGSOUND_H
#define DIALOGSOUND_H

#include <QDialog>

#include "../Widget/listitemsound.h"
#include "dialogtestaudio.h"

class Main;

namespace Ui {
class DialogSound;
}

class DialogSound : public DialogTestAudio
{
    Q_OBJECT

public:
    explicit DialogSound(Main* _main, ListItemSound* sound, bool creatingNew = false);
    ~DialogSound();
    void audioEngineInit();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonFile_clicked();
    void onClose();
    void on_pushButtonPlay_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();

    void on_lineEditFile_textEdited(const QString &arg1);

private:
    Ui::DialogSound* ui;
    Main* _main;
    ListItemSound* sound;
    bool creatingNew;
    bool soundUpdated;

    QString originalFileName;
    int originalVolume;

    void updateTestButtons();
    void updateFileName(QString fn);
};

#endif // DIALOGSOUND_H
