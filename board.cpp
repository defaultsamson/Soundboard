#include "board.h"
#include "mainwindow.h"
#include "sound.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QHotkey>
#include <QObject>

Board::Board(MainWindow *main, QString text) :
    QListWidgetItem(text),
    _key(-1),
    main(main),
    hotkey(new QHotkey(this->main))
{
    QObject::connect(this->hotkey, &QHotkey::activated, this->main, [&](){
        this->trigger();
    });
}

Board::~Board() {
    for (size_t i = 0; i < this->sounds.size(); ++i) delete this->sounds.at(i);
    delete hotkey;
}

void Board::setKey(int k) {
    this->_key = k;
    this->hotkey->setRegistered(false);
    if (k < 0) return;
    this->hotkey->setShortcut(QKeySequence(k), false);
    this->hotkey->setRegistered(true);
}

void Board::trigger() {
    // TODO switch boards

}

void Board::reg() {
    // TODO register all sounds keybinds
}

void Board::unreg() {
    // TODO register all sounds keybinds
}

int Board::key() {
    return this->_key;
}

void Board::addSound(Sound *sound) {
    this->sounds.push_back(sound);
}

void Board::removeSound(int n) {
    delete this->sounds.at(n);
    this->sounds.erase(this->sounds.begin() + n);
}

void Board::populateList(QListWidget *list) {
    // Removes items from list
    for (int i = list->count() - 1; i >= 0; --i) list->takeItem(i);
    // Adds items to list
    for (size_t i = 0; i < this->sounds.size(); ++i) list->addItem(this->sounds.at(i));
}

void Board::load(const QJsonObject &json) {
    this->setText(json["name"].toString());
    this->setKey(json["key"].toInt());

    QJsonArray arr = json["sounds"].toArray();

    // Loads all the sounds
    for (int i = 0; i < arr.size(); ++i) {
        Sound *sound = new Sound();
        sound->load(arr[i].toObject());
        this->addSound(sound);
    }
}

void Board::save(QJsonObject &json) {
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
