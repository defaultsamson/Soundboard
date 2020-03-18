#ifndef LISTITEMSOUND_H
#define LISTITEMSOUND_H

#include "listitem.h"
#include "../Audio/audioobjectfile.h"

#include <QString>
#include <QJsonObject>
#include <QFile>

class Main;
class ListItemBoard;

class ListItemSound : public ListItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(Main* _main, ListItemBoard* board);
    ~ListItemSound() override;
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void trigger() override;
    AudioObjectFile* audio();

private:
    ListItemBoard* board;
    AudioObjectFile* _audio;
};

#endif // LISTITEMSOUND_H
