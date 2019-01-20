#ifndef BOARDEDIT_H
#define BOARDEDIT_H

#include <QDialog>
#include <QWidget>

#include "board.h"

namespace Ui {
class BoardEdit;
}

class BoardEdit : public QDialog
{
    Q_OBJECT

public:
    explicit BoardEdit(QWidget *parent, Board *board);
    ~BoardEdit();

private slots:
    void on_pushButtonOK_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::BoardEdit *ui;
    Board *board;
};

#endif // BOARDEDIT_H
