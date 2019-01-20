#ifndef BOARD_H
#define BOARD_H

#include <QListWidgetItem>
#include <vector>
#include "sound.h"

class Board : public QListWidgetItem
{
public:
    Board(std::string text);
    ~Board();
    void addSound(Sound *sound);
    void removeSound(int n);
    void populateList(QListWidget *list);

private:
    std::vector<Sound *> sounds;
};

#endif // BOARD_H
