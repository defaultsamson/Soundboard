#ifndef DIALOGBOARD_H
#define DIALOGBOARD_H

#include <QDialog>
#include <QWidget>

#include "board.h"

namespace Ui {
class DialogBoard;
}

class DialogBoard : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBoard(QWidget *parent, Board *board);
    ~DialogBoard();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::DialogBoard *ui;
    Board *board;
};

#endif // DIALOGBOARD_H
