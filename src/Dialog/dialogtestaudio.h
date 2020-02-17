#ifndef DIALOGTESTAUDIO_H
#define DIALOGTESTAUDIO_H

#include <QDialog>

class DialogTestAudio : public QDialog
{
public:
    DialogTestAudio(QWidget* parent = nullptr) : QDialog(parent) {}
    virtual void audioEngineInit() = 0;
};

#endif // DIALOGTESTAUDIO_H
