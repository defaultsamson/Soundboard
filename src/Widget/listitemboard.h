#ifndef LISTITEMBOARD_H
#define LISTITEMBOARD_H

#include "listitemsound.h"
#include "listitem.h"

#include <QListWidget>
#include <QJsonObject>
#include <vector>
#include <QString>
#include <QHotkey>

class Main;

class ListItemBoard : public ListItem
{
public:
    static QString NEW_BOARD;
    ListItemBoard(Main* _main);
    ~ListItemBoard() override;
    void addSound(ListItemSound* sound);
    void removeSound(int n, bool deleteSound = true);
    void removeSound(ListItemSound* sound, bool deleteSound = true);
    int soundsCount();
    void moveSound(size_t from, size_t to);
    void populateList(QListWidget* list);
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void reg(bool regThis, bool regSounds);
    void unreg(bool unregThis);
    void trigger() override;

private:
    std::vector<ListItemSound*> sounds;
};

#endif // LISTITEMBOARD_H
