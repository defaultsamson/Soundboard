#include "listitemsound.h"
#include "../mainapp.h"

#include <QListWidgetItem>
#include <QJsonObject>
#include <QString>
#include <QHotkey>
#include <QUrl>
#include <iostream>

QString ListItemSound::NEW_SOUND = "New Sound";

ListItemSound::ListItemSound(Main *main, ListItemBoard *board) :
    ListItem(main),
    board(board),
    _filename(""),
    _volume(100),
    file(nullptr),
    _filenameChanged(false),
    _volumeChanged(false)
{
    setText(NEW_SOUND);
}

ListItemSound::~ListItemSound() {
    board->removeSound(this, false);
}

void ListItemSound::reload() {
    if (_filenameChanged) {
        //QAudioOutput *audio;
        //QFile f(filename());



        //audio.setSource(QUrl::fromUserInput(filename()));
    }
    if (_volumeChanged) {

        //qreal linearVolume = QAudio::convertVolume(_volume / qreal(100), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        //m_audioOutput->setVolume(linearVolume);

        //audio.setVolume(static_cast<double>(volume()) / static_cast<double>(100));
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
    _volumeChanged = false;
    reload();
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
    reload();
}

void ListItemSound::save(QJsonObject &json) {
    ListItem::save(json);
    json["filename"] = filename();
    json["volume"] = volume();
}

void ListItemSound::trigger() {
    ListItem::trigger();
    //audio.play();
    main->setCurrentSound(this);
    // TODO give this an AudioObject
}
