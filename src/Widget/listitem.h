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
    int key();
    int nativeKey();
    void setKey(int key);
    void setNativeKey(int key);
    virtual void load(const QJsonObject &json);
    virtual void save(QJsonObject &json);
    virtual void trigger() = 0;

protected:
    Main *main;
    int _key;
    int _native;
    QHotkey *hotkey;
};

#endif // LISTITEM_H
