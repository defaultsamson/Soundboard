#ifndef BOARD_H
#define BOARD_H

#include "listitemsound.h"
#include "listitem.h"

#include <QListWidget>
#include <QJsonObject>
#include <vector>
#include <QString>
#include <QHotkey>

class MainWindow;

class ListItemBoard : public ListItem
{
public:
    static QString NEW_BOARD;
    ListItemBoard(MainWindow *main);
    ~ListItemBoard() override;
    void addSound(ListItemSound *sound);
    void removeSound(int n, bool deleteSound = true);
    void removeSound(ListItemSound *sound, bool deleteSound = true);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void reg(bool regThis = false) override;
    void unreg(bool unregThis = false) override;
    void trigger() override;

private:
    MainWindow *main;
    std::vector<ListItemSound*> sounds;
};

#endif // BOARD_H
