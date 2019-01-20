#ifndef BOARD_H
#define BOARD_H

#include <QListWidgetItem>

class Board : public QListWidgetItem
{
public:
    Board(std::string text);
};

#endif // BOARD_H
