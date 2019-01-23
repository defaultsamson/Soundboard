#include "listitemsound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <iostream>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(MainWindow *main, ListItemBoard *board) :
    ListItem(main),
    board(board)
{
    setText(NEW_SOUND);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
}

void ListItemSound::setFileName(QString name) {
    filename = name;
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
}

void ListItemSound::save(QJsonObject &json) {
    ListItem::save(json);
    QJsonValue fn(filename);
    json["filename"] = fn;
}

void ListItemSound::trigger() {
    main->playSound(this);
}
