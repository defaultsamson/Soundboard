#ifndef DIALOGSOUND_H
#define DIALOGSOUND_H

#include <QDialog>

#include "listitemsound.h"

class MainWindow;

namespace Ui {
class DialogSound;
}

class DialogSound : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSound(MainWindow *main, ListItemSound *sound, bool creatingNew = false);
    ~DialogSound();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void onClose();

private:
    Ui::DialogSound *ui;
    ListItemSound *sound;
    MainWindow *main;
    bool didSomething = false;
    bool creatingNew;
};

#endif // DIALOGSOUND_H
