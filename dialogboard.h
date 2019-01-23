#ifndef DIALOGBOARD_H
#define DIALOGBOARD_H

#include <QDialog>

#include "listitemboard.h"

class MainWindow;

namespace Ui {
class DialogBoard;
}

class DialogBoard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBoard(MainWindow *main, ListItemBoard *board, bool creatingNew = false);
    ~DialogBoard();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

    void onClose();

private:
    Ui::DialogBoard *ui;
    MainWindow *main;
    ListItemBoard *board;
    bool creatingNew;
    bool didSomething = false;
};

#endif // DIALOGBOARD_H
