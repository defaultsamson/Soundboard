#include "listitemsound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <QSound>
#include <iostream>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(MainWindow *main, ListItemBoard *board) :
    ListItem(main),
    board(board),
    audio(nullptr)
{
    setText(NEW_SOUND);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
}

void ListItemSound::setFileName(QString name) {
    _filename = name;
    if (audio) delete audio;
    audio = new QSound(name);
}

QString ListItemSound::filename() {
    return _filename;
}

void ListItemSound::setVolume(int v) {
    _volume = v;
}

int ListItemSound::volume() {
    return _volume;
}

void ListItemSound::reg() {
    hotkey->setRegistered(true);
}

void ListItemSound::unreg() {
    hotkey->setRegistered(false);
}

void ListItemSound::load(const QJsonObject &json) {
    ListItem::load(json);
    setFileName(json["filename"].toString());
    setVolume(json["volume"].toInt());
}

void ListItemSound::save(QJsonObject &json) {
    ListItem::save(json);
    json["filename"] = filename();
    json["volume"] = volume();
}

void ListItemSound::trigger() {
    audio->play();
    main->playSound(this);
}
