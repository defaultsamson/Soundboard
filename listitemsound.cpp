#include "listitemsound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <iostream>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(MainWindow *main, ListItemBoard *board) :
    QListWidgetItem(),
    main(main),
    board(board),
    _key(-1),
    hotkey(new QHotkey(this->main))
{
    setText(NEW_SOUND);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
}

void ListItemSound::setFileName(QString name) {
    this->filename = name;
}

void ListItemSound::reg() {
    this->hotkey->setRegistered(true);
}

void ListItemSound::unreg() {
    this->hotkey->setRegistered(false);
}

void ListItemSound::load(const QJsonObject &json) {
    this->setText(json["name"].toString());
    // TODO this->setKey(json["key"].toInt());
    this->setFileName(json["filename"].toString());
}

void ListItemSound::save(QJsonObject &json) {
    json["name"] = this->text();
    QJsonValue fn(this->filename);
    json["filename"] = fn;
}
