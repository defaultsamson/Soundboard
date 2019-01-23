#include "listitemboard.h"

#include "mainwindow.h"
#include "listitemsound.h"
#include "listitem.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHotkey>
#include <QObject>

#include <iostream>

QString ListItemBoard::NEW_BOARD = "New Board";

ListItemBoard::ListItemBoard(MainWindow *main) :
    ListItem(main),
    main(main)
{
    setText(NEW_BOARD);
}

ListItemBoard::~ListItemBoard() {
    for (size_t i = 0; i < sounds.size(); ++i) delete sounds.at(i);
}

void ListItemBoard::addSound(ListItemSound *sound) {
    sounds.push_back(sound);
}

void ListItemBoard::removeSound(int n, bool deleteSound) {
    if (n < 0) return;
    if (deleteSound) delete sounds.at(n);
    sounds.erase(sounds.begin() + n);
}

void ListItemBoard::removeSound(ListItemSound *sound, bool deleteSound) {
    int n = -1;
    std::vector<ListItemSound *>::iterator iter = std::find(sounds.begin(), sounds.end(), sound);
    if(iter != sounds.end())
    {
        n = std::distance(sounds.begin(), iter);
    }
    removeSound(n, deleteSound);
}

void ListItemBoard::populateList(QListWidget *list) {
    // Removes items from list
    for (int i = list->count() - 1; i >= 0; --i) list->takeItem(i);
    // Adds items to list
    for (size_t i = 0; i < sounds.size(); ++i) list->addItem(sounds.at(i));
}

void ListItemBoard::load(const QJsonObject &json) {
    ListItem::load(json);

    QJsonArray arr = json["sounds"].toArray();

    // Loads all the sounds
    for (int i = 0; i < arr.size(); ++i) {
        ListItemSound *sound = new ListItemSound(main, this);
        sound->load(arr[i].toObject());
        addSound(sound);
    }
}

void ListItemBoard::save(QJsonObject &json) {
    ListItem::save(json);

    QJsonArray sounds;
    for (size_t i = 0; i < sounds.size(); ++i) {
        QJsonObject s;
        this->sounds.at(i)->save(s);
        sounds.append(s);
    }

    json["sounds"] = sounds;
}

void ListItemBoard::reg(bool regThis) {
    ListItem::reg(regThis);
    // Register all sounds' keybinds
    for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->reg();
}

void ListItemBoard::unreg(bool unregThis) {
    ListItem::unreg(unregThis);
    // Unregister all sounds' keybinds
    for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->unreg();
}

void ListItemBoard::trigger() {
    main->setCurrentBoard(this);
}
