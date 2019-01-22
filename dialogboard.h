#ifndef DIALOGBOARD_H
#define DIALOGBOARD_H

#include <QDialog>
#include <QWidget>

#include "listitemboard.h"

namespace Ui {
class DialogBoard;
}

class DialogBoard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBoard(QWidget *parent, ListItemBoard *board);
    ~DialogBoard();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogBoard *ui;
    ListItemBoard *board;
};

#endif // DIALOGBOARD_H
