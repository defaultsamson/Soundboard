#ifndef LISTITEM_H
#define LISTITEM_H

#include <QListWidgetItem>
#include <QHotkey>

class MainWindow;

class ListItem : public QListWidgetItem
{
public:
    ListItem(MainWindow *main);
    ~ListItem();
    int key();
    void setKey(int key);
    virtual void load(const QJsonObject &json);
    virtual void save(QJsonObject &json);
    virtual void trigger() = 0;

protected:
    int _key;
    QHotkey *hotkey;
};

#endif // LISTITEM_H
