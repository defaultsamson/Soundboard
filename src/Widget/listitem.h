#ifndef LISTITEM_H
#define LISTITEM_H

#include <QListWidgetItem>
#include <QHotkey>
#include <QObject>

class Main;

class ListItem : public QListWidgetItem, public QObject
{
public:
    ListItem(Main *main);
    ~ListItem();
    bool hasKey();
    quint32 key();
    void unSetKey();
    void setKey(quint32 key);
    virtual void load(const QJsonObject &json);
    virtual void save(QJsonObject &json);
    virtual void trigger() = 0;

protected:
    Main *main;
    bool _hasKey;
    quint32 _key;
    QHotkey *hotkey;
};

#endif // LISTITEM_H
