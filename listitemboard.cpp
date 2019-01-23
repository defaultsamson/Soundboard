#include "listitemboard.h"
#include "mainwindow.h"
#include "listitemsound.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHotkey>
#include <QObject>

#include <iostream>

QString ListItemBoard::NEW_BOARD = "New Board";

ListItemBoard::ListItemBoard(MainWindow *main) :
    QListWidgetItem(),
    _key(-1),
    main(main),
    hotkey(new QHotkey(main))
{
    setText(NEW_BOARD);

    QObject::connect(this->hotkey, &QHotkey::activated, main, [&](){
        main->setCurrentBoard(this);
    });
}

ListItemBoard::~ListItemBoard() {
    for (size_t i = 0; i < sounds.size(); ++i) delete sounds.at(i);
    delete hotkey;
}

void ListItemBoard::setKey(int k) {
    _key = k;
    hotkey->setRegistered(false);
    if (k < 0) return;
    hotkey->setShortcut(QKeySequence(k), false);
    hotkey->setRegistered(true);
}

void ListItemBoard::reg(bool regThis) {
    if (regThis && _key >= 0) hotkey->setRegistered(true);
    // Register all sounds' keybinds
    for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->reg();
}

void ListItemBoard::unreg(bool unregThis) {
    if (unregThis) hotkey->setRegistered(false);
    // Unregister all sounds' keybinds
    for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->unreg();
}

int ListItemBoard::key() {
    return this->_key;
}

void ListItemBoard::addSound(ListItemSound *sound) {
    sounds.push_back(sound);
}

void ListItemBoard::removeSound(int n) {
    if (n < 0) return;
    delete sounds.at(n);
    sounds.erase(sounds.begin() + n);
}

void ListItemBoard::removeSound(ListItemSound *sound) {
    int index = -1;
    for (size_t i = 0; i < sounds.size(); ++i) {
        if (sound == sounds.at(i)) {
            index = i;
            break;
        }
    }
    if (index >= 0) {
        removeSound(index);
    }
}

void ListItemBoard::populateList(QListWidget *list) {
    // Removes items from list
    for (int i = list->count() - 1; i >= 0; --i) list->takeItem(i);
    // Adds items to list
    for (size_t i = 0; i < sounds.size(); ++i) list->addItem(sounds.at(i));
}

void ListItemBoard::load(const QJsonObject &json) {
    setText(json["name"].toString());
    setKey(json["key"].toInt());

    QJsonArray arr = json["sounds"].toArray();

    // Loads all the sounds
    for (int i = 0; i < arr.size(); ++i) {
        ListItemSound *sound = new ListItemSound(main, this);
        sound->load(arr[i].toObject());
        addSound(sound);
    }
}

void ListItemBoard::save(QJsonObject &json) {
    json["name"] = text();
    json["key"] = key();

    QJsonArray sounds;
    for (size_t i = 0; i < sounds.size(); ++i) {
        QJsonObject s;
        this->sounds.at(i)->save(s);
        sounds.append(s);
    }

    json["sounds"] = sounds;
}
