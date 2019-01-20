#ifndef BOARD_H
#define BOARD_H

#include <QListWidgetItem>
#include <vector>
#include "sound.h"

class Board : public QListWidgetItem
{
public:
    Board(QString text = "New Board");
    ~Board();
    void addSound(Sound *sound);
    void removeSound(int n);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);

private:
    std::vector<Sound*> sounds;
};

#endif // BOARD_H
