#include "listitemsound.h"

#include "mainwindow.h"
#include "listitemboard.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(MainWindow *main, ListItemBoard *board) :
    QListWidgetItem(),
    main(main),
    _board(board),
    _key(-1),
    hotkey(new QHotkey(this->main))
{
    setText(NEW_SOUND);
}

void ListItemSound::setFileName(QString name) {
    filename = name;
}

void ListItemSound::setKey(int k) {
    _key = k;
    hotkey->setRegistered(false);
    if (k < 0) return;
    hotkey->setShortcut(QKeySequence(k), false);
    hotkey->setRegistered(true);
}

ListItemBoard *ListItemSound::board() {
    return _board;
}

int ListItemSound::key() {
    return _key;
}

void ListItemSound::reg() {
    if (_key >= 0) hotkey->setRegistered(true);
}

void ListItemSound::unreg() {
    hotkey->setRegistered(false);
}

void ListItemSound::load(const QJsonObject &json) {
    setText(json["name"].toString());
    setKey(json["key"].toInt());
    setFileName(json["filename"].toString());
}

void ListItemSound::save(QJsonObject &json) {
    json["name"] = text();
    json["key"] = key();
    QJsonValue fn(filename);
    json["filename"] = fn;
}
