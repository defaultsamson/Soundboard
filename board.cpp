#include "board.h"
#include <QListWidgetItem>

Board::Board(std::string text) :
    QListWidgetItem(QString::fromStdString(text))
{

}
