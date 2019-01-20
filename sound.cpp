#include "sound.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>

Sound::Sound(QString text) :
    QListWidgetItem(text)
{

}

void Sound::setFileName(QString name) {
    this->filename = name;
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
