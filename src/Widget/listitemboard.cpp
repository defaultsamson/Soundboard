#include "listitemboard.h"

#include "../mainapp.h"
#include "listitemsound.h"
#include "listitem.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHotkey>
#include <QObject>

QString ListItemBoard::NEW_BOARD = "New Board";

ListItemBoard::ListItemBoard(Main* main) :
    ListItem(main)
{
    setText(NEW_BOARD);
}

ListItemBoard::~ListItemBoard() {
    if (sounds.size() > 0) {
        size_t i = sounds.size();
        while (i--) {
            delete sounds.at(i);
            if (i == 0) break;
        }
        sounds.clear();
    }
}

void ListItemBoard::addSound(ListItemSound* sound) {
    sounds.push_back(sound);
}

void ListItemBoard::removeSound(int n, bool deleteSound) {
    if (n < 0) return;
    if (deleteSound) delete sounds.at(static_cast<size_t>(n));
    sounds.erase(sounds.begin() + n);
}

void ListItemBoard::removeSound(ListItemSound* sound, bool deleteSound) {
    int n = -1;
    std::vector<ListItemSound*>::iterator iter = std::find(sounds.begin(), sounds.end(), sound);
    if(iter != sounds.end())
    {
        n = static_cast<int>(std::distance(sounds.begin(), iter));
    }
    removeSound(n, deleteSound);
}

void ListItemBoard::moveSound(size_t from, size_t to) {
    if (from >= sounds.size() || to >= sounds.size()) return;
    if (from < to) {
        // Moves "from" right to "to", shifting everything inbeween to the left by 1
        for (size_t i = from; i < to; ++i) std::swap(sounds[i], sounds[i + 1]);
    } else {
        // Moves "from" left to "to", shifting everything inbeween to the right by 1
        for (size_t i = from; i > to; --i) std::swap(sounds[i], sounds[i - 1]);
    }
}

int ListItemBoard::soundsCount() {
    return static_cast<int>(sounds.size());
}

void ListItemBoard::populateList(QListWidget* list) {
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
        ListItemSound* sound = new ListItemSound(main, this);
        sound->load(arr[i].toObject());
        addSound(sound);
    }
}

void ListItemBoard::save(QJsonObject &json) {
    ListItem::save(json);

    QJsonArray arr;
    for (size_t i = 0; i < sounds.size(); ++i) {
        QJsonObject s;
        sounds.at(i)->save(s);
        arr.append(s);
    }

    json["sounds"] = arr;
}

void ListItemBoard::reg(bool regThis, bool regSounds) {
    if (regThis) _hotkey->setRegistered(true);
    // Register all sounds' keybinds
    if (regSounds) for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->reg();
}

void ListItemBoard::unreg(bool unregThis) {
    if (unregThis) _hotkey->setRegistered(false);
    // Unregister all sounds' keybinds
    for (size_t i = 0; i < sounds.size(); ++i) sounds.at(i)->unreg();
}

void ListItemBoard::trigger() {
    main->setCurrentBoard(this);
}
