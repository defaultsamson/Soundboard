#include "listitemsound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>


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
    // TODO register all sounds keybinds
}

void ListItemSound::unreg() {
    // TODO register all sounds keybinds
}

void ListItemSound::load(const QJsonObject &json) {
    this->setText(json["name"].toString());
    this->setFileName(json["filename"].toString());
}

void ListItemSound::save(QJsonObject &json) {
    json["name"] = this->text();
    QJsonValue fn(this->filename);
    json["filename"] = fn;
}
