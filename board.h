#ifndef BOARD_H
#define BOARD_H

#include <QListWidgetItem>
#include <vector>
#include "sound.h"
#include <QObject>
#include <QHotkey>

class Board : public QListWidgetItem
{
public:
    Board(QObject *main, QString text = "New Board");
    ~Board();
    void addSound(Sound *sound);
    void removeSound(int n);
    void populateList(QListWidget *list);
    void load(const QJsonObject &json);
    void save(QJsonObject &json);
    int key();
    void setKey(int key);

private:
    std::vector<Sound*> sounds;
    int _key;
    QObject *main;
    QHotkey *hotkey;
    void trigger();
};

#endif // BOARD_H
