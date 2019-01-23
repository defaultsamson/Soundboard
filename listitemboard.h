#ifndef BOARD_H
#define BOARD_H

#include "listitemsound.h"

#include <QListWidgetItem>
#include <QListWidget>
#include <QJsonObject>
#include <vector>
#include <QString>
#include <QHotkey>

class MainWindow;

class ListItemBoard : public QListWidgetItem
{
public:
    static QString NEW_BOARD;
    ListItemBoard(MainWindow *main);
    ~ListItemBoard();
    void addSound(ListItemSound *sound);
    void removeSound(int n);
    void removeSound(ListItemSound *sound);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    int key();
    void setKey(int key);
    void reg(bool regThis = false);
    void unreg(bool unregThis = false);

private:
    MainWindow *main;

    std::vector<ListItemSound*> sounds;

    int _key;
    QHotkey *hotkey;
};

#endif // BOARD_H
