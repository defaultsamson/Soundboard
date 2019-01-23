#ifndef SOUND_H
#define SOUND_H

#include "listitem.h"

#include <QString>
#include <QJsonObject>
#include <QHotkey>

class MainWindow;
class ListItemBoard;

class ListItemSound : public ListItem
{
public:
    static QString NEW_SOUND;
    ListItemSound(MainWindow *main, ListItemBoard *board);
    ~ListItemSound() override;
    void setFileName(QString name);
    void load(const QJsonObject &json) override;
    void save(QJsonObject &json) override;
    void reg();
    void unreg() ;
    void trigger() override;

private:
    ListItemBoard *board;
    QString filename;
};

#endif // SOUND_H
