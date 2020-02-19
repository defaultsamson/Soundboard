#ifndef LISTITEM_H
#define LISTITEM_H

#include <QListWidgetItem>
#include <QHotkey>
#include "../Hotkey/hotkeytrigger.h"

class Main;

class ListItem : public QListWidgetItem, public HotkeyTrigger
{
public:
    ListItem(Main* _main);
    virtual void load(const QJsonObject &json);
    virtual void save(QJsonObject &json);
    virtual void trigger() = 0;
};

#endif // LISTITEM_H
