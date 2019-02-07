#ifndef BOARD_H
#define BOARD_H

#include "listitemsound.h"
#include "listitem.h"

#include <QListWidget>
#include <QJsonObject>
#include <vector>
#include <QString>

class Main;

class ListItemBoard : public ListItem
{
public:
    static QString NEW_BOARD;
    ListItemBoard(Main *main);
    ~ListItemBoard() override;
    void addSound(ListItemSound *sound);
    void removeSound(int n, bool deleteSound = true);
    void removeSound(ListItemSound *sound, bool deleteSound = true);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void reg(bool regThis, bool regSounds);
    void unreg(bool unregThis);
    void trigger() override;

private:
    std::vector<ListItemSound*> sounds;
};

#endif // BOARD_H
