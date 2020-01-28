#ifndef SOUND_H
#define SOUND_H

#include "listitem.h"

#include <QString>
#include <QJsonObject>
#include <QHotkey>
#include <QFile>

#include "../Audio/audioobject.h"

class Main;
class ListItemBoard;

class ListItemSound : public ListItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(Main *main, ListItemBoard *board);
    ~ListItemSound() override;
    void setFileName(QString name);
    QString filename();
    void setVolume(int v);
    int volume();
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void reg();
    void unreg() ;
    void trigger() override;
    AudioObject *audio();

private:
    ListItemBoard *board;
    AudioObject *_audio;
    QString _filename;
    int _volume;
    bool _filenameChanged;
    bool _volumeChanged;
    void reload();
};

#endif // SOUND_H
