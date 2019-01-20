#ifndef BOARD_H
#define BOARD_H

#include "sound.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <vector>
#include <QString>
#include <QHotkey>

class MainWindow;

class Board : public QListWidgetItem
{
public:
    Board(MainWindow *main, QString text = "New Board");
    ~Board();
    void addSound(Sound *sound);
    void removeSound(int n);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    int key();
    void setKey(int key);
    void reg();
    void unreg();

private:
    std::vector<Sound*> sounds;

    int _key;
    MainWindow *main;
    QHotkey *hotkey;
};

#endif // BOARD_H
