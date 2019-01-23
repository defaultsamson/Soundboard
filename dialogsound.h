#ifndef DIALOGSOUND_H
#define DIALOGSOUND_H

#include <QDialog>

namespace Ui {
class DialogSound;
}

class DialogSound : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSound(QWidget *parent = nullptr);
    ~DialogSound();

private:
    Ui::DialogSound *ui;
};

#endif // DIALOGSOUND_H
