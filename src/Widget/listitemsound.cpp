#include "listitemsound.h"
#include "../mainapp.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <QUrl>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(Main* _main, ListItemBoard* board) :
    ListItem(_main),
    board(board),
    _audio(new AudioObjectFile())
{
    setText(NEW_SOUND);
    _main->audio().registerAudio(_audio);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
    _main->audio().unregisterAudio(_audio);
    delete _audio;
}

void ListItemSound::load(const QJsonObject &json) {
    ListItem::load(json);
    _audio->setFile(json["filename"].toString());
    _audio->setVolumeInt(json["volume"].toInt());
}

void ListItemSound::save(QJsonObject &json) {
    ListItem::save(json);
    json["filename"] = _audio->filename();
    json["volume"] = _audio->volumeInt();
}

AudioObjectFile* ListItemSound::audio() {
    return _audio;
}

void ListItemSound::trigger() {
    _audio->play();
    _main->setCurrentSound(this);
}
