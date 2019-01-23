#ifndef SOUND_H
#define SOUND_H

#include "listitem.h"

#include <QString>
#include <QJsonObject>
#include <QHotkey>
#include <QSound>

class MainWindow;
class ListItemBoard;

class ListItemSound : public ListItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(MainWindow *main, ListItemBoard *board);
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

private:
    ListItemBoard *board;
    QString _filename;
    int _volume;
    QSound *audio;
};

#endif // SOUND_H
