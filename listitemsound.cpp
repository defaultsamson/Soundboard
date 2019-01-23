#include "listitemsound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(MainWindow *main, QString text) :
    QListWidgetItem(text),
    _key(-1),
    main(main),
    hotkey(new QHotkey(this->main))
{

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
