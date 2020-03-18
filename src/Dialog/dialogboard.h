#ifndef DIALOGBOARD_H
#define DIALOGBOARD_H

#include "../Widget/listitemboard.h"

#include <QDialog>

class Main;

namespace Ui {
class DialogBoard;
}

class DialogBoard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBoard(Main* _main, ListItemBoard* _board, bool creatingNew = false);
    ~DialogBoard();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void onClose();

private:
    Ui::DialogBoard* ui;
    Main* _main;
    ListItemBoard* _board;
    bool creatingNew;
    bool boardUpdated = false;
};

#endif // DIALOGBOARD_H
