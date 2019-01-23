#include "listitemboard.h"
#include "mainwindow.h"
#include "listitemsound.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHotkey>
#include <QObject>

QString ListItemBoard::NEW_BOARD = "New Board";

ListItemBoard::ListItemBoard(MainWindow *main, QString text) :
    QListWidgetItem(text),
    _key(-1),
    main(main),
    hotkey(new QHotkey(this->main))
{
    QObject::connect(this->hotkey, &QHotkey::activated, this->main, [&](){
        this->main->setCurrentBoard(this);
    });
}

ListItemBoard::~ListItemBoard() {
    for (size_t i = 0; i < this->sounds.size(); ++i) delete this->sounds.at(i);
    delete hotkey;
}

void ListItemBoard::setKey(int k) {
    this->_key = k;
    this->hotkey->setRegistered(false);
    if (k < 0) return;
    this->hotkey->setShortcut(QKeySequence(k), false);
    this->hotkey->setRegistered(true);
}

void ListItemBoard::reg(bool regThis) {
    if (regThis) this->hotkey->setRegistered(true);
    // Register all sounds' keybinds
    for (size_t i = 0; i < this->sounds.size(); ++i) this->sounds.at(i)->reg();
}

void ListItemBoard::unreg(bool unregThis) {
    if (unregThis) this->hotkey->setRegistered(false);
    // Unregister all sounds' keybinds
    for (size_t i = 0; i < this->sounds.size(); ++i) this->sounds.at(i)->unreg();
}

int ListItemBoard::key() {
    return this->_key;
}

void ListItemBoard::addSound(ListItemSound *sound) {
    this->sounds.push_back(sound);
}

void ListItemBoard::removeSound(int n) {
    if (n < 0) return;
    delete this->sounds.at(n);
    this->sounds.erase(this->sounds.begin() + n);
}

void ListItemBoard::populateList(QListWidget *list) {
    // Removes items from list
    for (int i = list->count() - 1; i >= 0; --i) list->takeItem(i);
    // Adds items to list
    for (size_t i = 0; i < this->sounds.size(); ++i) list->addItem(this->sounds.at(i));
}

void ListItemBoard::load(const QJsonObject &json) {
    this->setText(json["name"].toString());
    this->setKey(json["key"].toInt());

    QJsonArray arr = json["sounds"].toArray();

    // Loads all the sounds
    for (int i = 0; i < arr.size(); ++i) {
        ListItemSound *sound = new ListItemSound(this->main);
        sound->load(arr[i].toObject());
        this->addSound(sound);
    }
}

void ListItemBoard::save(QJsonObject &json) {
    json["name"] = this->text();
    json["key"] = this->key();

    QJsonArray sounds;
    for (size_t i = 0; i < this->sounds.size(); ++i) {
        QJsonObject s;
        this->sounds.at(i)->save(s);
        sounds.append(s);
    }

    json["sounds"] = sounds;
}
