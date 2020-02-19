#include "listitemsound.h"
#include "../mainapp.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <QUrl>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(Main* main, ListItemBoard* board) :
    ListItem(main),
    board(board),
    _audio(new AudioObjectFile()),
    _filename(""),
    _volume(100),
    _filenameChanged(false),
    _volumeChanged(false)
{
    setText(NEW_SOUND);
    main->audio()->registerAudio(_audio);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
    main->audio()->unregisterAudio(_audio);
    delete _audio;
}

void ListItemSound::reload() {
    if (_filenameChanged) {
        _audio->setFile(filename()); // QUrl::fromUserInput(filename())
    }
    if (_volumeChanged) {
        _audio->setVolume(_volume / static_cast<float>(100)); //qreal linearVolume = QAudio::convertVolume(_volume / qreal(100), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    }
    _filenameChanged = false;
    _volumeChanged = false;
}

void ListItemSound::setFileName(QString name) {
    _filename = name;
    _filenameChanged = true;
    reload();
}

QString ListItemSound::filename() {
    return _filename;
}

void ListItemSound::setVolume(int v) {
    _volume = v;
    _volumeChanged = true;
    reload();
}

int ListItemSound::volume() {
    return _volume;
}

void ListItemSound::reg() {
    _hotkey->setRegistered(true);
}

void ListItemSound::unreg() {
    _hotkey->setRegistered(false);
}

void ListItemSound::load(const QJsonObject &json) {
    ListItem::load(json);
    setFileName(json["filename"].toString());
    setVolume(json["volume"].toInt());
    reload();
}

void ListItemSound::save(QJsonObject &json) {
    ListItem::save(json);
    json["filename"] = filename();
    json["volume"] = volume();
}

AudioObjectFile* ListItemSound::audio() {
    return _audio;
}

void ListItemSound::trigger() {
    _audio->play();
    main->setCurrentSound(this);
}
