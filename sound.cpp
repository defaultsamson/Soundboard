#include "sound.h"
#include "mainwindow.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>


Sound::Sound(MainWindow *main, QString text) :
    QListWidgetItem(text),
    _key(-1),
    main(main),
    hotkey(new QHotkey(this->main))
{

}

void Sound::setFileName(QString name) {
    this->filename = name;
}

void Sound::reg() {
    // TODO register all sounds keybinds
}

void Sound::unreg() {
    // TODO register all sounds keybinds
}

void Sound::load(const QJsonObject &json) {
    this->setText(json["name"].toString());
    this->setFileName(json["filename"].toString());
}

void Sound::save(QJsonObject &json) {
    json["name"] = this->text();
    QJsonValue fn(this->filename);
    json["filename"] = fn;
}
