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
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onClose();

private:
    Ui::DialogBoard *ui;
    MainWindow *main;
    ListItemBoard *board;
    bool creatingNew;
    bool boardUpdated;
};

#endif // DIALOGBOARD_H
