#include "listitem.h"

#include "mainwindow.h"
#include <QHotkey>
#include <QObject>

ListItem::ListItem(MainWindow *main):
    _key(-1),
    hotkey(new QHotkey(main))
{
    QObject::connect(this->hotkey, &QHotkey::activated, main, [&](){
        main->trigger(this);
    });
}

ListItem::~ListItem() {
    delete hotkey;
}

void ListItem::setKey(int k) {
    _key = k;
    // hotkey->setRegistered(false);
    if (k < 0) return;
    hotkey->setShortcut(QKeySequence(k), false);
    // hotkey->setRegistered(true);
}
void ListItem::load(const QJsonObject &json) {
    setText(json["name"].toString());
    setKey(json["key"].toInt());
}

void ListItem::save(QJsonObject &json) {
    json["name"] = text();
    json["key"] = key();
}

void ListItem::reg(bool regThis) {
    if (regThis) hotkey->setRegistered(true);
}

void ListItem::unreg(bool unregThis) {
    if (unregThis) this->hotkey->setRegistered(false);
}

int ListItem::key() {
    return this->_key;
}
