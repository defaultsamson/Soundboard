#ifndef DIALOGBOARD_H
#define DIALOGBOARD_H

#include "listitemboard.h"

#include <QDialog>
#include <QCloseEvent>

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
    ListItemBoard *board;
    MainWindow *main;
    bool didSomething = false;
    bool creatingNew;
};

#endif // DIALOGBOARD_H
